#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cerrno>

#include "MainWindow.h"
#include "ViewTextDialog.h"
#include "polyfitgsl.h"

#include <gsl/gsl_statistics_double.h>

#include <QtCore/QUuid>

#include <QtGui/QMenuBar>
#include <QtGui/QMenu>

#include <QtGui/QMessageBox>
#include <QtGui/QHeaderView>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>
#include <QSqlDatabase>

#include <QTextStream>
#include <QFileDialog>
#include <QDir>

#include <QDateTime>
#include <QSplitter>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QInputDialog>

#include <QHostInfo>
#include <QProcess>
#include <QRegExp>

#include <qwt_plot.h>
#include <qwt_legend.h>
#include <qwt_symbol.h>
//#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>

#include <QtCore/QList>
#include "PolyEval.h"

#define MAX_BATH_PROBES 10
#define PATH_TO_BATH_CALS "/Configuration/raf/cal_files/Bath"
#define DECADE_BOX_WIRE_RESISTANCE 0.03 // Ohms
//#define DECADE_BOX_WIRE_RESISTANCE 0  // Remove resistance when requested by Friesen


/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
#include <QItemDelegate>

class BackgroundColorDelegate: public QItemDelegate
{
public:
    BackgroundColorDelegate(QAbstractItemModel *model, QObject *parent = 0)
      : QItemDelegate(parent)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        _model = model;
    }

public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        drawBackground(painter, option, index);
        QItemDelegate::paint(painter, option, index);
    }

    void highlightRow(QString rid, QColor color)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << "rid: " << rid.toStdString();
        std::cout << " color: " << color.red() << " " << color.green() << " " << color.blue() << std::endl;
        m_colorMap[rid] = color;
    }
    void unhighlightRow(QString rid)
    {
        std::cout << "rid: " << rid.toStdString() << std::endl;
        m_colorMap.remove(rid);
    }

protected:
    void drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QString rid = _model->index(index.row(), clm_rid).data().toString().trimmed();
        if (m_colorMap.contains(rid))
            painter->fillRect(option.rect, m_colorMap.value(rid));
    }
private:
    QAbstractItemModel *_model;

    /// model's row id (rid) to color mapping; temporary model data. 
    QMap<QString, QColor> m_colorMap;
};
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */

MainWindow::MainWindow() : _model(0), changeDetected(false)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // Seed the random number generator used to create UUIDs.
    qsrand(time(0));
    qDebug() << QUuid::createUuid();
    qDebug() << QUuid::createUuid();
    qDebug() << QUuid::createUuid();

    setupDatabase();

    setupModels();

    setupTable();

    setupDelegates();

    setupViews();

    setupMenus();

    setWindowTitle(tr("Edit Calibration Database"));
    resize(870, 550);

    hideRows();
}

/* -------------------------------------------------------------------- */

void MainWindow::setupDatabase()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // Ping the master DB server to see if it is active.
    QProcess process;
    QStringList params;
    params << DB_HOST << "-i" << "1" << "-w" << "1" <<"-c" << "1";

    if (process.execute("ping", params)) {
        QMessageBox::information(0, tr("pinging calibration database"),
          tr("cannot contact:\n") + DB_HOST);
        exit(1);
    }
    // List of remote sites that fill a calibration database.
    QStringList siteList;
    siteList << "thumper.eol.ucar.edu";
    siteList << "hyper.raf-guest.ucar.edu";
    siteList << "hercules.raf-guest.ucar.edu";

    tailNumIdx[0] = "Lab_N600";
    tailNumIdx[1] = "GV_N677F";
    tailNumIdx[2] = "C130_N130AR";
    tailNumIdx[3] = "Lab_FL1";

    // deny editing local calibration databases
    foreach(QString site, siteList) {
        if (QHostInfo::localHostName() == site) {
            QMessageBox::information(0, tr("denied"),
              tr("cannot edit local calibration database on:\n") + site);
            exit(1);
        }
    }
    // extract some environment variables
    calfile_dir = QString::fromAscii(getenv("PROJ_DIR")) +
                         "/Configuration/raf/cal_files/";
    csvfile_dir = QString::fromAscii(getenv("PWD")) +
                         "/";

#ifdef SANDBOX
    // point sandboxed version to someplace benign
    calfile_dir = QString::fromAscii(getenv("HOME")) + "/";
    csvfile_dir = QString::fromAscii(getenv("HOME")) + "/";
#endif

    qDebug() << "calfile_dir: " << calfile_dir;
    qDebug() << "csvfile_dir: " << csvfile_dir;

    // prompt user if they want to pull data from the sites at start up
    QMessageBox::StandardButton reply = QMessageBox::question(0, tr("Pull"),
      tr("Pull calibration databases from the sites?\n"),
      QMessageBox::Yes | QMessageBox::No);

    openDatabase(DB_HOST);

    if (reply == QMessageBox::Yes)
        foreach(QString site, siteList)
            importRemoteCalibTable(site);

    // re-sort and save database after importing before use
    saveButtonClicked();
}

/* -------------------------------------------------------------------- */

void MainWindow::setupModels()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    _model = new QSqlTableModel;

    // detect changes to model
    connect(_model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this,     SLOT(dataChanged(const QModelIndex&, const QModelIndex&)));

    _model->setTable(DB_TABLE);
    _model->setSort(clm_cal_date, Qt::AscendingOrder);
    _model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    _model->select();

    _proxy = new SortFilterProxyModel;
    _proxy->setDynamicSortFilter(true); // NOTE - http://doc.qt.digia.com/4.7-snapshot/qsortfilterproxymodel.html#dynamicSortFilter-prop
    _proxy->setSourceModel(_model);
}

/* -------------------------------------------------------------------- */

void MainWindow::setupTable()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    _table = new QTableView;
// TODO use a custom delegate for displaying the clm_cal_date?
//
// http://www.qtcentre.org/archive/index.php/t-20277.html mentions this...
// The proper way of handling such situations is to provide a custom QAbstractItemDelegate for the widget mapper with setModelData() and setEditorData() reimplemented.
//
// these are inherited members of QTableView:
// void setItemDelegateForColumn(int column, QAbstractItemDelegate *delegate);
// QAbstractItemDelegate *itemDelegateForColumn(int column) const;

    // disable editing of table
    _table->setEditTriggers(QAbstractItemView::NoEditTriggers);

/* TODO this doesn't show anything... yet the columns are still draggable.  why?  
    _table->setDragEnabled(true);
    _table->setAcceptDrops(true);
    _table->setDropIndicatorShown(true);
*/
    _table->setModel(_proxy);

    _table->setContextMenuPolicy( Qt::CustomContextMenu );

    QHeaderView *hH = _table->horizontalHeader();
    hH->setContextMenuPolicy( Qt::CustomContextMenu );

    _table->adjustSize();

    connect(_table, SIGNAL(          pressed(const QModelIndex &)),
            this,     SLOT( tableItemPressed(const QModelIndex &)));

    connect(hH,     SIGNAL( customContextMenuRequested( const QPoint & )),
            this,     SLOT(             showHeaderMenu( const QPoint & )));

    connect(_table, SIGNAL( customContextMenuRequested( const QPoint & )),
            this,     SLOT(              showTableMenu( const QPoint & )));
}

/* -------------------------------------------------------------------- */

void MainWindow::setupDelegates()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    _delegate = new BackgroundColorDelegate(_proxy);
    _table->setItemDelegate(_delegate);
}

/* -------------------------------------------------------------------- */

void MainWindow::setupViews()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    QSplitter *splitHorz = new QSplitter;
    QSplitter *splitVert = new QSplitter;

    splitHorz->setOrientation(Qt::Horizontal);
    splitVert->setOrientation(Qt::Vertical);

    QWidget *container = new QWidget;
    QVBoxLayout *layoutHorz = new QVBoxLayout;

    _plot = new CalibrationPlot(splitHorz);
    _form = new CalibrationForm(splitHorz);

    splitHorz->addWidget(_plot);
    splitHorz->addWidget(_form);

    layoutHorz->addWidget(splitHorz);
    container->setLayout(layoutHorz);

    splitVert->addWidget(container);
    splitVert->addWidget(_table);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(splitVert);

    QWidget *wnd = new QWidget;
    wnd->setLayout(layout);
    setCentralWidget(wnd);

//   _plot->setModel(_proxy); // not used
     _form->setModel(_proxy);

     _form->setEnabled(false);

// TODO is this needed?
//   QItemSelectionModel *selectionModel = new QItemSelectionModel(_proxy);
//   _table->setSelectionModel(selectionModel);
//   _form->setSelectionModel( _table->selectionModel() );
//   _form->setSelectionModel(selectionModel);
//
    connect(_form, SIGNAL(removeSetPoint(int ,int)),
            this,    SLOT(removeSetPoint(int ,int)));

    connect(_form, SIGNAL(changeFitButtonClicked(int, int)),
            this,    SLOT(changeFitButtonClicked(int, int)));

    connect(_form, SIGNAL(replot(int)),
            this,    SLOT(replot(int)));

    connect(_form, SIGNAL(initializeForm(int)),
            this,    SLOT(initializeForm(int)));

    connect(_form, SIGNAL(submitForm(int)),
            this,    SLOT(submitForm(int)));

    connect(_form->_selectRid, SIGNAL(pressed()),
            this,                SLOT(scrollToEditedRow()));

    connect(this, SIGNAL(submitForm()),
            _form,  SLOT(submit()));

    connect(this, SIGNAL(revertForm()),
            _form,  SLOT(revert()));

    for (int i=0; i < _proxy->columnCount(); i++)
        _table->resizeColumnToContents(i);

    QHeaderView *horizontalHeader = _table->horizontalHeader();
    horizontalHeader->setMovable(true);
    horizontalHeader->setStretchLastSection(true);
    horizontalHeader->setResizeMode(QHeaderView::Interactive);

    QHeaderView *verticalHeader = _table->verticalHeader();
    verticalHeader->setResizeMode(QHeaderView::Fixed);
    verticalHeader->hide();
}

/* -------------------------------------------------------------------- */

MainWindow::~MainWindow()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    onQuit();

    delete _delegate;
    delete _table;
    delete _proxy;
    delete _model;

    // re-enforce uniqueness constraint (HACK - this gets dropped during 
    // simple "open... no edit... close" situations).
    QString cmd( "ALTER TABLE ONLY "DB_TABLE" ADD CONSTRAINT "DB_TABLE"_rid_key UNIQUE (rid)");
    qDebug() << "cmd:" << cmd;
    QSqlQuery query(cmd);
    query.exec();
    query.finish();

    QSqlDatabase::database().close();

    std::cout << __PRETTY_FUNCTION__ << " EXITING" << std::endl;
}

/* -------------------------------------------------------------------- */

void MainWindow::showHeaderMenu( const QPoint &pos )
{
    // clear any multiple selections made by user
    _table->selectionModel()->clearSelection();

    QHeaderView *hH = _table->horizontalHeader();
    _column = hH->logicalIndexAt(pos);
    std::cout << __PRETTY_FUNCTION__ << " _column: " << _column << std::endl;

    // Popup table menu setup... (cannot use keyboard shortcuts here)
    QMenu *headerMenu = new QMenu;
    QString column = _model->headerData(_column, Qt::Horizontal).toString();
    QString filterBy    = tr("Filter '%1' by...").arg(column);
    QString unfilterAll = tr("Unfilter all columns");
    QString hideColumn  = tr("Hide '%1' column").arg(column);
    QString showColumn  = tr("Show column...");

    headerMenu->addAction(filterBy,    this, SLOT(filterBy()));
    headerMenu->addAction(unfilterAll, this, SLOT(unfilterAll()));
    hideColumnAction = headerMenu->addAction(hideColumn,  this, SLOT(hideColumn()));
    showColumnAction = headerMenu->addAction(showColumn,  this, SLOT(showColumn()));

    connect(headerMenu, SIGNAL(aboutToShow()), this, SLOT(headerMenu_aboutToShow()));

    // show the popup menu
    headerMenu->exec( _table->mapToGlobal(pos) + QPoint(20,0) );
}

/* -------------------------------------------------------------------- */

void MainWindow::filterBy()
{
    QString column = _model->headerData(_column, Qt::Horizontal).toString();
    QString filterBy = tr("Filter '%1' by...").arg(column);

    bool ok;
    QString pattern = QInputDialog::getText(this, filterBy,
                           tr("Pressing OK with nothing entered unfilters this column."),
                           QLineEdit::Normal, _proxy->filterAt(_column), &ok);

    if (!ok) return;

    setFilter( _column, pattern);
}

/* -------------------------------------------------------------------- */

void MainWindow::setFilter(int column, const QString &pattern)
{
    _proxy->setFilter(column, pattern);
}

/* -------------------------------------------------------------------- */

void MainWindow::unfilterAll()
{
    _proxy->clearFilters();
}

/* -------------------------------------------------------------------- */

void MainWindow::headerMenu_aboutToShow()
{
    int nChecked = 0;
    foreach (QAction *action, colsGrp->actions())
        if (action->isChecked())
            nChecked++;

    hideColumnAction->setEnabled(nChecked != 1);
    showColumnAction->setEnabled(nChecked != colsGrp->actions().length());
}

/* -------------------------------------------------------------------- */

void MainWindow::hideColumn()
{
    _table->setColumnHidden(_column, true);
    QList<QAction*> actions = colsGrp->actions();
    actions[_column]->setChecked(false);
}

/* -------------------------------------------------------------------- */

void MainWindow::showColumn()
{
    QStringList allColumns, columns;
    foreach (QAction *action, colsGrp->actions()) {
        allColumns << action->text();
        if (!action->isChecked())
            columns << action->text();
    }
    bool ok;
    QString column = QInputDialog::getItem(this, tr("Show Column..."),
                           tr("currently hidden columns:"),
                           columns, 0, false, &ok);

    if (!ok) return;

    int sC = allColumns.indexOf(column);

    _table->setColumnHidden(sC, false);
    QList<QAction*> actions = colsGrp->actions();
    actions[sC]->setChecked(true);
}

/* -------------------------------------------------------------------- */

void MainWindow::showTableMenu( const QPoint &pos )
{
    // clear any multiple selections made by user
    _table->selectionModel()->clearSelection();

    // select the row
    int row = _table->indexAt(pos).row();
    _table->selectionModel()->select(_proxy->index(row, 0),
      QItemSelectionModel::Select | QItemSelectionModel::Rows);

    // show the popup menu
    tableMenu->exec( _table->mapToGlobal(pos) + QPoint(20,0) );
}

/* -------------------------------------------------------------------- */

void MainWindow::removeSetPoint(int row, int index)
{
    std::cout << __PRETTY_FUNCTION__ << " row: " << row << " index: " << index << std::endl;

    QStringList list_set_times  = extractListFromBracedCSV(form_set_times);
    QStringList list_set_points = extractListFromBracedCSV(form_set_points);
    QStringList list_averages   = extractListFromBracedCSV(form_averages);
    QStringList list_stddevs    = extractListFromBracedCSV(form_stddevs);

    // mitigate old data that predates the use of set_times
    if (list_set_times.count() > 1)
        list_set_times.replace(index, "");
    list_set_points.replace(index, "");
    list_averages.replace(index, "");
    list_stddevs.replace(index, "");

    // modify a set of temporary variables setup for the form representation
    form_set_times  = "{" + list_set_times.join(",") + "}";
    form_set_points = "{" + list_set_points.join(",") + "}";
    form_averages   = "{" + list_averages.join(",") + "}";
    form_stddevs    = "{" + list_stddevs.join(",") + "}";

    restrictOrderChoice(list_set_points);

    std::cout << "form_set_times:  " << form_set_times.toStdString() << std::endl;
    std::cout << "form_set_points: " << form_set_points.toStdString() << std::endl;
    std::cout << "form_averages:   " << form_averages.toStdString() << std::endl;
    std::cout << "form_stddevs:    " << form_stddevs.toStdString() << std::endl;
}

/* -------------------------------------------------------------------- */

void MainWindow::restrictOrderChoice(QStringList list_set_points)
{
    // restrict allowable polynomial order radio buttons
    _form->_linearRB->setEnabled(false);
    _form->_2ndOrderRB->setEnabled(false);
    _form->_3rdOrderRB->setEnabled(false);
    int nSP = 0;
    foreach(QString sp, list_set_points)
        if (!sp.isEmpty()) nSP++;
    int max_order = nSP-1;
    std::cout << "max_order: " << max_order << std::endl;
    if (max_order > 2) _form->_3rdOrderRB->setEnabled(true);
    if (max_order > 1) _form->_2ndOrderRB->setEnabled(true);
    if (max_order > 0) _form->_linearRB->setEnabled(true);
}

/* -------------------------------------------------------------------- */

void MainWindow::replot(int row)
{
    std::cout << __PRETTY_FUNCTION__ << " row: " << row << std::endl;

    // don't plot if not already plotted
    QString rid = modelData(row, clm_rid);
    foreach (CalibrationCurve *curve, plottedCurves)
        if (curve->rid == rid) {
            unplotCalButtonClicked(row);
            plotCalButtonClicked(row);
            return;
        }
}

/* -------------------------------------------------------------------- */

void MainWindow::addRowAction(QMenu *menu, const QString &text,
                              QActionGroup *group, QSignalMapper *mapper,
                              int id, bool checked)
{
    int n = 0;
    if      (id == n++) showAnalog     = checked;
    else if (id == n++) showBath       = checked;
    else if (id == n++) showInstrument = checked;
    else if (id == n++) showTailNum[0] = checked;
    else if (id == n++) showTailNum[1] = checked;
    else if (id == n++) showTailNum[2] = checked;
    else if (id == n++) showTailNum[3] = checked;
    else if (id == n++) showCloned     = checked;
    else if (id == n++) showRemoved    = checked;
    else if (id == n++) showExported   = checked;

    addAction(menu, text, group, mapper, id, checked);
}

/* -------------------------------------------------------------------- */

void MainWindow::addColAction(QMenu *menu, const QString &text,
                              QActionGroup *group, QSignalMapper *mapper,
                              int id, bool checked)
{
    _model->setHeaderData(id, Qt::Horizontal, text);
    _table->setColumnHidden(id, !checked);

    addAction(menu, text, group, mapper, id, checked);
}

/* -------------------------------------------------------------------- */

void MainWindow::addAction(QMenu *menu, const QString &text,
                           QActionGroup *group, QSignalMapper *mapper,
                           int id, bool checked)
{
    QAction *result = menu->addAction(text);
    result->setCheckable(true);
    result->setChecked(checked);
    group->addAction(result);

    QObject::connect(result, SIGNAL(triggered()), mapper, SLOT(map()));
    mapper->setMapping(result, id);
}

/* -------------------------------------------------------------------- */

inline QString MainWindow::modelData(int row, int col)
{
    return _proxy->index(row, col).data().toString().trimmed();
}

/* -------------------------------------------------------------------- */

QStringList MainWindow::extractListFromBracedCSV(int row, calTableHeaders key)
{
    QString string = modelData(row, key);
    return extractListFromBracedCSV(string);
}

/* -------------------------------------------------------------------- */

QStringList MainWindow::extractListFromBracedCSV(QString string)
{
    QStringList list;

    list = extractStringWithinBraced(string).split(",");

    return list;
}

/* -------------------------------------------------------------------- */

QString MainWindow::extractStringWithinBraced(QString string)
{
    QRegExp rxBraced("\\{(.*)\\}");
    QString inside;

    if (rxBraced.indexIn(string) != -1)
        inside = rxBraced.cap(1);

    return inside;
}

/* -------------------------------------------------------------------- */

void MainWindow::tableItemPressed(const QModelIndex &)
{
//  std::cout << __PRETTY_FUNCTION__ << std::endl;

    // get selected row number
    int row = _table->selectionModel()->currentIndex().row();

    _lastRid = modelData(row, clm_rid);
}

/* -------------------------------------------------------------------- */

void MainWindow::toggleRow(int id)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    _table->selectionModel()->clearSelection();

    // Toggle the row's hidden state
    int n = 0;
    if      (id == n++) showAnalog     = !showAnalog;
    else if (id == n++) showBath       = !showBath;
    else if (id == n++) showInstrument = !showInstrument;
    else if (id == n++) showTailNum[0] = !showTailNum[0];
    else if (id == n++) showTailNum[1] = !showTailNum[1];
    else if (id == n++) showTailNum[2] = !showTailNum[2];
    else if (id == n++) showTailNum[3] = !showTailNum[3];
    else if (id == n++) showCloned     = !showCloned;
    else if (id == n++) showRemoved    = !showRemoved;
    else if (id == n++) showExported   = !showExported;

    hideRows();
}

/* -------------------------------------------------------------------- */

void MainWindow::hideRows()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    QStringList cal_type_filters;
    if (showAnalog)     cal_type_filters << "analog";
    if (showBath)       cal_type_filters << "bath";
    if (showInstrument) cal_type_filters << "instrument";
    if (cal_type_filters.isEmpty())
        setFilter(clm_cal_type, "");
    else
        setFilter(clm_cal_type, cal_type_filters.join("|"));

    QStringList tailNumbers;
    if (showTailNum[0]) tailNumbers << tailNumIdx[0];
    if (showTailNum[1]) tailNumbers << tailNumIdx[1];
    if (showTailNum[2]) tailNumbers << tailNumIdx[2];
    if (showTailNum[3]) tailNumbers << tailNumIdx[3];
    if (tailNumbers.isEmpty())
        setFilter(clm_site, "");
    else
        setFilter(clm_site, tailNumbers.join("|"));

    if (showCloned)
        setFilter(clm_pid, "");
    else
        setFilter(clm_pid, " ");

    if (showRemoved)
        setFilter(clm_removed, "");
    else
        setFilter(clm_removed, "0");

    if (showExported)
        setFilter(clm_exported, "");
    else
        setFilter(clm_exported, "0");

    _proxy->refreshFilters();
}

/* -------------------------------------------------------------------- */

void MainWindow::toggleColumn(int id)
{
    _table->setColumnHidden(id, !_table->isColumnHidden(id));
    _table->resizeColumnToContents(id);

    // reduce the size of some of the larger columns
    if (id == clm_comment)
        _table->setColumnWidth(id, 300);
}

/* -------------------------------------------------------------------- */

void MainWindow::setupMenus()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // Popup table menu setup... (cannot use keyboard shortcuts here)
    tableMenu = new QMenu;
    tableMenu->addAction(tr("Edit Cal"),                  this, SLOT(editCalButtonClicked()));
    tableMenu->addAction(tr("Plot Cal"),                  this, SLOT(plotCalButtonClicked()));
    tableMenu->addAction(tr("Unplot Cal"),                this, SLOT(unplotCalButtonClicked()));
    tableMenu->addAction(tr("Unplot All Cals"),           this, SLOT(unplotAllButtonClicked()));
    tableMenu->addAction(tr("Export to Cal File"),        this, SLOT(exportCalButtonClicked()));
    tableMenu->addAction(tr("Export to CSV File"),        this, SLOT(exportCsvButtonClicked()));
    tableMenu->addAction(tr("View Cal File"),             this, SLOT(viewCalButtonClicked()));
    tableMenu->addAction(tr("View CSV File"),             this, SLOT(viewCsvButtonClicked()));
    tableMenu->addAction(tr("Clone Entry"),               this, SLOT(cloneButtonClicked()));
    tableMenu->addAction(tr("Delete Entry"),              this, SLOT(removeButtonClicked()));

    // File menu setup...
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(tr("&Import"), this, SLOT(importButtonClicked()), Qt::CTRL + Qt::Key_I);
    fileMenu->addAction(tr("&Save"),   this, SLOT(saveButtonClicked()),   Qt::CTRL + Qt::Key_S);
    fileMenu->addAction(tr("&Quit"),   this, SLOT(onQuit()),              Qt::CTRL + Qt::Key_Q);
//  connect(this, SIGNAL(aboutToQuit()), this, SLOT(onQuit()));
    menuBar()->addMenu(fileMenu);

    QMenu *viewMenu = new QMenu(tr("&View"));

    // View->Rows menu setup...
    QSignalMapper *rowsMapper = new QSignalMapper(this);
    connect(rowsMapper, SIGNAL(mapped(int)), this, SLOT(toggleRow(int)));

    QActionGroup *rowsGrp = new QActionGroup(this);
    rowsGrp->setExclusive(false);

    QMenu *rowsMenu = new QMenu(tr("&Rows"));

    // true == unhidden
    int i = 0;
    rowsMenu->addAction(tr("select &start of table"),  this, SLOT(scrollToHome()),        Qt::Key_Home);
    rowsMenu->addAction(tr("select &end of table"),    this, SLOT(scrollToEnd()),         Qt::Key_End);
    rowsMenu->addAction(tr("select &last clicked"),    this, SLOT(scrollToLastClicked()), Qt::Key_ScrollLock);
    rowsMenu->addSeparator();
    addRowAction(rowsMenu, tr("analog"),        rowsGrp, rowsMapper, i++, true);
    addRowAction(rowsMenu, tr("bath"),          rowsGrp, rowsMapper, i++, true);
    addRowAction(rowsMenu, tr("instrument"),    rowsGrp, rowsMapper, i++, true);
    rowsMenu->addSeparator();
    addRowAction(rowsMenu, tailNumIdx[0],       rowsGrp, rowsMapper, i++, true);
    addRowAction(rowsMenu, tailNumIdx[1],       rowsGrp, rowsMapper, i++, true);
    addRowAction(rowsMenu, tailNumIdx[2],       rowsGrp, rowsMapper, i++, true);
    addRowAction(rowsMenu, tailNumIdx[3],       rowsGrp, rowsMapper, i++, true);
    rowsMenu->addSeparator();
    addRowAction(rowsMenu, tr("cloned"),        rowsGrp, rowsMapper, i++, true);
    addRowAction(rowsMenu, tr("removed"),       rowsGrp, rowsMapper, i++, false);
    addRowAction(rowsMenu, tr("exported"),      rowsGrp, rowsMapper, i++, true);

    viewMenu->addMenu(rowsMenu);

    // View->Columns menu setup...
    QSignalMapper *colsMapper = new QSignalMapper(this);
    connect(colsMapper, SIGNAL(mapped(int)), this, SLOT(toggleColumn(int)));

    colsGrp = new QActionGroup(this);
    colsGrp->setExclusive(false);

    QMenu *colsMenu = new QMenu(tr("&Columns"));

    // true == unhidden
    i = 0;
    addColAction(colsMenu, tr("Row Id"),        colsGrp, colsMapper, i++, false); // rid
    addColAction(colsMenu, tr("Parent Id"),     colsGrp, colsMapper, i++, false); // pid
    addColAction(colsMenu, tr("Site"),          colsGrp, colsMapper, i++, true);  // Site
    addColAction(colsMenu, tr("Pulled"),        colsGrp, colsMapper, i++, false); // pulled
    addColAction(colsMenu, tr("Removed"),       colsGrp, colsMapper, i++, true);  // removed
    addColAction(colsMenu, tr("Exported"),      colsGrp, colsMapper, i++, true);  // exported
    addColAction(colsMenu, tr("Date"),          colsGrp, colsMapper, i++, true);  // cal_date
    addColAction(colsMenu, tr("Project"),       colsGrp, colsMapper, i++, true);  // project_name
    addColAction(colsMenu, tr("User"),          colsGrp, colsMapper, i++, false); // username
    addColAction(colsMenu, tr("Sensor Type"),   colsGrp, colsMapper, i++, false); // sensor_type
    addColAction(colsMenu, tr("Serial #"),      colsGrp, colsMapper, i++, true);  // serial_number
    addColAction(colsMenu, tr("Variable"),      colsGrp, colsMapper, i++, true);  // var_name
    addColAction(colsMenu, tr("DSM"),           colsGrp, colsMapper, i++, false); // dsm_name
    addColAction(colsMenu, tr("Cal Type"),      colsGrp, colsMapper, i++, true);  // cal_type
    addColAction(colsMenu, tr("Channel"),       colsGrp, colsMapper, i++, false); // channel
    addColAction(colsMenu, tr("GainBplr"),      colsGrp, colsMapper, i++, false); // gainbplr
    addColAction(colsMenu, tr("ADS file name"), colsGrp, colsMapper, i++, false); // ads_file_name
    addColAction(colsMenu, tr("Set Times"),     colsGrp, colsMapper, i++, false); ///set_times
    addColAction(colsMenu, tr("Set Points"),    colsGrp, colsMapper, i++, false); ///set_points
    addColAction(colsMenu, tr("Avg Values"),    colsGrp, colsMapper, i++, false); ///averages
    addColAction(colsMenu, tr("StdDev Values"), colsGrp, colsMapper, i++, false); ///stddevs
    addColAction(colsMenu, tr("Calibration"),   colsGrp, colsMapper, i++, true);  // cal
    addColAction(colsMenu, tr("Temperature"),   colsGrp, colsMapper, i++, false); // temperature
    addColAction(colsMenu, tr("Comment"),       colsGrp, colsMapper, i++, false); // comment

    viewMenu->addMenu(colsMenu);

    menuBar()->addMenu(viewMenu);
}

/* -------------------------------------------------------------------- */

void MainWindow::openDatabase(QString hostname)
{
    std::cout << __PRETTY_FUNCTION__ << " hostname: " << hostname.toStdString() << std::endl;

    QSqlDatabase db = QSqlDatabase::addDatabase(DB_DRIVER);
    db.setDatabaseName(DB_NAME);
    db.setHostName(hostname);
    db.setUserName(DB_USER);
    if (!db.open()) {

        std::ostringstream ostr;
        ostr << tr("Failed to open calibration database on:\n").toStdString();
        ostr << hostname.toStdString() << std::endl;
        QString errMsg =  db.lastError().text();
        if (errMsg.contains("FATAL:  too many connections for database ")) {

            // determine who is currently accessing it
            db.setDatabaseName("postgres");
            if (db.open()) {

                QSqlQuery query(QSqlDatabase::database());
                QString cmd("SELECT usename,client_addr from pg_stat_activity WHERE datname='calibrations'");

                QString currentUser = errMsg;
                if (query.exec(cmd) && query.first())
                    currentUser = "\nCurrent user: " + query.value(0).toString()
                                + "\nLogged in from: " + query.value(1).toString();
                query.finish();
                ostr << tr(currentUser.toStdString().c_str()).toStdString();
            } else {
                ostr << tr("Cannot determine who has the database open.").toStdString();
            }
        } else {
            // unknown error
            ostr << tr(errMsg.toAscii().data()).toStdString();
        }
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(hostname);

        QMessageBox::critical(0, tr("open"), ostr.str().c_str());
        exit(1);
    }
}

/* -------------------------------------------------------------------- */

void MainWindow::dataChanged(const QModelIndex& old, const QModelIndex& now)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    changeDetected = true;
}

/* -------------------------------------------------------------------- */

QString MainWindow::createRID()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // initialize a unique RID
    QSqlQuery query(QSqlDatabase::database());
    QUuid uuid_rid;
    QString rid;
    bool unique = false;
    QString chk;
    // HACK - loop until an actual unique one is found (relative to this system)
    while(not unique) {
        uuid_rid = QUuid::createUuid();
        rid = extractStringWithinBraced(uuid_rid.toString());
        chk = QString("SELECT rid FROM calibrations "
                      "WHERE rid='%1'").arg(rid); //.c_str());
        qDebug() << "chk: " << chk;
        unique = not (query.exec(chk) && query.last());
        if (not unique)
            qDebug() << "not unique: " << rid;
    }
    return rid;
}

/* -------------------------------------------------------------------- */

void MainWindow::unsubmittedFormQuery(QString title)
{
    if (_form->_revertBtn->isEnabled()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(0, title,
                    tr("Unsubmitted changes detected in previous edit.\nSubmit them?\n"),
                    QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
            emit submitForm();
        else
            emit revertForm();
        replot(_form->getRow());
    }
}

/* -------------------------------------------------------------------- */

void MainWindow::onQuit()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    unsubmittedFormQuery(tr("Close"));
    if (changeDetected) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(0, tr("Close"),
                    tr("Save changes to database?\n"),
                    QMessageBox::Yes | QMessageBox::Discard);

        if (reply == QMessageBox::Yes)
            saveButtonClicked();
    }
    close();
}

/* -------------------------------------------------------------------- */

void MainWindow::importRemoteCalibTable(QString remote)
{
    std::cout << __PRETTY_FUNCTION__ << " remote: " << remote.toStdString() << std::endl;

    QProcess process;
    QStringList params;
    QSqlQuery query(QSqlDatabase::database());

    // Ping the remote DB server to see if it is active.
    params << remote << "-i" << "1" << "-w" << "1" <<"-c" << "1";

    if (process.execute("ping", params)) {
        QMessageBox::information(0, tr("pinging calibration database"),
          tr("cannot contact:\n") + remote);
        return;
    }
    QString connectStr = QString("'host=%1 user=ads password=snoitarbilac "
                       "dbname="DB_NAME"'").arg(remote);
    qDebug() << "connectStr:" << connectStr;

    QString insertStr = QString("INSERT INTO "DB_TABLE" SELECT * FROM dblink(%1, "
      "'SELECT * FROM "DB_TABLE" WHERE pulled=\\'0\\' ORDER BY cal_date') AS ("
      "rid character(36),"
      "pid character(36),"
      "site character varying(10),"
      "pulled character(1),"
      "removed character(1),"
      "exported character(1),"
      "cal_date timestamp without time zone,"
      "project_name character varying(32),"
      "username character varying(32),"
      "sensor_type character varying(20),"
      "serial_number character varying(20),"
      "var_name character varying(20),"
      "dsm_name character varying(16),"
      "cal_type character varying(16),"
      "channel character(1),"
      "gainbplr character(2),"
      "ads_file_name character varying(200),"
      "set_times timestamp without time zone[],"
      "set_points double precision[],"
      "averages double precision[],"
      "stddevs double precision[],"
      "cal double precision[],"
      "temperature double precision,"
      "comment character varying(256));").arg(connectStr);
    qDebug() << "insertStr:" << insertStr;

    QString pulledStr = QString(
      "SELECT cal_date, site, var_name FROM "DB_TABLE""
      " WHERE pulled=\'0\' ORDER BY cal_date");
    qDebug() << "pulledStr:" << pulledStr;

    QString updateMasterStr = QString(
      "UPDATE "DB_TABLE" SET pulled=\\'1\\' WHERE pulled=\\'0\\'");
    qDebug() << "updateMasterStr:" << updateMasterStr;

    QString updateRemoteStr = QString("SELECT * FROM dblink_exec(%1, '%2')")
      .arg(connectStr, updateMasterStr);
    qDebug() << "updateRemoteStr:" << updateRemoteStr;

    updateMasterStr.replace("\\", "");
    qDebug() << "updateMasterStr:" << updateMasterStr;

    // insert unpulled rows from remote database
    qDebug() << "insert unpulled rows from remote database";
    if (!query.exec(insertStr)) {
        QMessageBox::information(0, tr("remote database query failed"),
          query.lastError().text());
        return;
    }
    query.finish();

    // briefly show what was just pulled in
    qDebug() << "briefly show what was just pulled in";
    std::ostringstream brief;
    QSqlQuery briefQuery(pulledStr);
    while (briefQuery.next()) {
        for (int i=0; i<3; i++)
            brief << briefQuery.value(i).toString().toStdString() << " ";
        brief << "\n";
    }
    if (brief.tellp())
        viewText( QString(brief.str().c_str()), tr("pulled records"));

    // mark rows as pulled on remote database
    qDebug() << "mark rows as pulled on remote database";
    if (!query.exec(updateRemoteStr)) {
        QMessageBox::information(0, tr("remote database update failed"),
          query.lastError().text());
        return;
    }
    query.finish();

    // mark rows as pulled on master database
    qDebug() << "mark rows as pulled on master database";
    if (!query.exec(updateMasterStr)) {
        QMessageBox::information(0, tr("master database update failed"),
          query.lastError().text());
        return;
    }
    query.finish();
}

/* -------------------------------------------------------------------- */

int MainWindow::importButtonClicked()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // provide a file dialog for selecting the file
    QString filename = QFileDialog::getOpenFileName(this,
      tr("Import a bath calibration file"),
      getenv("PROJ_DIR")+QString(PATH_TO_BATH_CALS), "");
    if ( filename.isEmpty() ) return 1;
    qDebug() << filename;

/** EXAMPLE data file:
3/21/2012 11:37 AM
nDataPerSetPoint: 30
Pre-DC3 temperatue run with RAF probes 03-21-2012
Reference(C), Harco 708094A(Ohm), Harco 708094B(Ohm), Rosemount 2984(Ohm)
29.866024 	55.899364 	55.903518 	56.526749 
...
*/
    QString calDate;
    QString comment;
    QStringList sensor_types;
    QStringList serial_numbers;
    int nVariables = 0;
    QStringList list_set_points;
    QStringList list_averages[MAX_BATH_PROBES];
    QStringList list_stddevs[MAX_BATH_PROBES];

    // parse the file's data
    QFile file(filename);
    if (file.open(QFile::ReadOnly)) {
        calDate              = file.readLine().trimmed();
        int nDataPerSetPoint = QString(file.readLine()).section(": ", 1).toInt();
        comment              = file.readLine().trimmed();
        QString header       = file.readLine().trimmed();
        qDebug() << calDate;
        qDebug() << nDataPerSetPoint;
        qDebug() << comment;
        qDebug() << header;
        QStringList sensors = header.split(",");
        QRegExp rxVarName(" (.*) (.*)\\((.*)\\)");

        // trim off the initial 'Reference(C)' from the list of sensors
        sensors.removeFirst();

        nVariables = sensors.length();
        foreach (QString varName, sensors) {
            if (rxVarName.indexIn(varName) != -1) {
                sensor_types   << rxVarName.cap(1);
                serial_numbers << rxVarName.cap(2);
                QString units   = rxVarName.cap(3); // unused
            }
        }
        double setPointVal[nDataPerSetPoint];
        double value[nVariables][nDataPerSetPoint];
        QString line;
        QStringList list;

        int n = 0;
        while(!file.atEnd()) {
            line = file.readLine();
//          qDebug() << "line: " << line;
            list = line.simplified().split(" ");

            setPointVal[n] = list.at(0).toDouble();

            for (int c=0; c<nVariables; c++)
                value[c][n] = list.at(c+1).toDouble();

            if (++n == nDataPerSetPoint) {
                double setPoint = gsl_stats_mean (setPointVal, 1, nDataPerSetPoint);
                QString aQStr;
                aQStr.sprintf("%7.3f", setPoint);
//              qDebug() << "setPoint:" << aQStr;
                list_set_points << aQStr;

                for (int c=0; c<nVariables; c++) {
                    double average = gsl_stats_mean (value[c], 1, nDataPerSetPoint);
                    double stddev  = gsl_stats_sd   (value[c], 1, nDataPerSetPoint);
                    aQStr.sprintf("%7.3f", average);
//                  qDebug() << "average: " << aQStr;
                    list_averages[c] << aQStr;
                    aQStr.sprintf("%9.6f", stddev);
//                  qDebug() << "stddev:  " << aQStr;
                    list_stddevs[c]  << aQStr;
                }
                n = 0;
            }
        }
    }
    else {
        QMessageBox::information(0, tr("notice"),
          tr("missing:\n") + filename + tr("\n\nNot found."));
        return 1;
    }
    QString averages[nVariables];
    QString stddevs[nVariables];
    QString cals[nVariables];
    QString project_name;

    // prompt user for details about the imported calibrations
    QStringListModel* projects = _form->setupComboModel("project_name");

    qDebug() << "sensor_types:   " << sensor_types;
    qDebug() << "serial_numbers: " << serial_numbers;
    bool ok;

    do {
        comment = QInputDialog::getText(this, tr("edit comment?"),
          tr("current comment:\n")+comment, QLineEdit::Normal, comment, &ok);
    } while ((!ok) || (comment.length() == 0));

    do {
        project_name = QInputDialog::getItem(this, tr("project name"),
          comment, projects->stringList(), 0, true, &ok);
    } while ((!ok) || (project_name.length() == 0));

    qDebug() << "project_name: " << project_name;

    // setup for generating calibration fits for storage
    int nSetPoints = list_set_points.count();
    double setp[nSetPoints];
    double avrg[nSetPoints];

    for (int i=0; i<nSetPoints; i++)
        setp[i] = list_set_points[i].toDouble();

    // wrap gathered results as braced CSV strings
    QString set_points = "{" + list_set_points.join(",") + "}";
    qDebug() << "set_points: " << set_points;

    for (int c=0; c<nVariables; c++) {
        averages[c]   = "{" + list_averages[c].join(",") + "}";
        stddevs[c]    = "{" + list_stddevs[c].join(",") + "}";

        for (int i=0; i<nSetPoints; i++)
            avrg[i] = list_averages[c][i].toDouble();

        // generate a quadratic fit to start with
        double coeff[MAX_ORDER], Rsq;
        QStringList list_coeffs;
        polynomialfit(nSetPoints, 3, setp, avrg, coeff, &Rsq);
        for (int f=0; f<3; f++)
            list_coeffs << QString::number( coeff[f] );
        cals[c]   = "{" + list_coeffs.join(",") + "}";

        qDebug() << "averages[" << c << "]:  " << averages[c];
        qDebug() << "stddevs[" << c << "]:   " << stddevs[c];
        qDebug() << "cals[" << c << "]:      " << cals[c];

        // implementation similar to cloneButtonClicked...
        QString rid           = createRID();
        QDateTime cal_date    = QDateTime::fromString(calDate, "M/d/yyyy h:mm AP");
    
        QSqlRecord record = _model->record();
    
        // stuff the new record
        record.setValue(clm_rid,           rid);
        record.setValue(clm_pid,           "");
        record.setValue(clm_site,          "Lab_FL1");
        record.setValue(clm_pulled,        "1");
        record.setValue(clm_removed,       "0");
        record.setValue(clm_exported,      "0");
        record.setValue(clm_cal_date,      cal_date);
        record.setValue(clm_project_name,  project_name);
        record.setValue(clm_username,      getenv("USERNAME"));
        record.setValue(clm_sensor_type,   sensor_types[c]);
        record.setValue(clm_serial_number, serial_numbers[c]);
        record.setValue(clm_var_name,      "TT_BATH");
        record.setValue(clm_dsm_name,      "");
        record.setValue(clm_cal_type,      "bath");
        record.setValue(clm_channel,       QString::number(c));
        record.setValue(clm_gainbplr,      "");
        record.setValue(clm_ads_file_name, filename);
        record.setValue(clm_set_times,     "{}");
        record.setValue(clm_set_points,    set_points);
        record.setValue(clm_averages,      averages[c]);
        record.setValue(clm_stddevs,       stddevs[c]);
        record.setValue(clm_cal,           cals[c]);
        record.setValue(clm_temperature,   "nan");
        record.setValue(clm_comment,       comment);
    
        // TODO find a row based upon cal_date
        int row = -1;  // HACK insert at top of table for now
        _model->insertRow(row+1);
        _model->setRecord(row+1, record);

        // re-apply any filtering after inserting a new row
        _lastRid = rid;
        qDebug() << "_lastRid:" << _lastRid;
        _proxy->refreshFilters();
        scrollToLastClicked();
    }
    // Adding a new row(s) does not trigger a dataChanged event
    changeDetected = true;

    return 0;
}

/* -------------------------------------------------------------------- */

int MainWindow::saveButtonClicked()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // update calibration database 
    if (_model && !_model->submitAll()) {
        QString lastError = _model->lastError().text();
        QSqlDatabase::database().rollback();
        QMessageBox::warning(0, tr("save"),
           tr("The database reported an error: %1") .arg(lastError));
        return 1;
    }
    // resort table by cal_date
    std::cout << "resort table by cal_date" << std::endl;
    QSqlQuery query(QSqlDatabase::database());

    QString script =
      "CREATE TABLE resorted (LIKE "DB_TABLE");"
      "INSERT INTO resorted SELECT * FROM "DB_TABLE" ORDER BY cal_date;"
      "DROP TABLE "DB_TABLE";"
      "CREATE TABLE "DB_TABLE" (LIKE resorted);"
      "INSERT INTO "DB_TABLE" SELECT * FROM resorted;"
      "DROP TABLE resorted;";

    QStringList scriptQueries = script.split(';');

    // TODO create a progress bar for this loop?
    foreach (QString queryTxt, scriptQueries) {
        if (queryTxt.trimmed().isEmpty()) continue;
        qDebug() << "queryTxt:" << queryTxt;
        if (!query.exec(queryTxt)) {
            QMessageBox::warning(0, tr("resort"),
              query.lastError().text());
            return 1;
        }
        query.finish();
    }
    changeDetected = false;
    return 0;
}

/* -------------------------------------------------------------------- */

void MainWindow::scrollToHome()
{
    QModelIndex index = _proxy->index(0, 0);
    _table->scrollTo(index, QAbstractItemView::EnsureVisible);
    _table->selectRow(index.row());
}

/* -------------------------------------------------------------------- */

void MainWindow::scrollToEnd()
{
    QModelIndex index = _proxy->index(_proxy->rowCount()-1, 0);
    _table->scrollTo(index, QAbstractItemView::EnsureVisible);
    _table->selectRow(index.row());
}

/* -------------------------------------------------------------------- */

void MainWindow::scrollToLastClicked()
{
    scrollToRid(_lastRid);
}

/* -------------------------------------------------------------------- */

void MainWindow::scrollToEditedRow()
{
    scrollToRid(_editRid);
}

/* -------------------------------------------------------------------- */

void MainWindow::scrollToRid(QString rid)
{
    QModelIndex index;
    for (int row = 0; row < _proxy->rowCount(); row++)
        if (modelData(row, clm_rid) == rid)
            index = _proxy->index(row, 0);

    if (!index.isValid()) {
        QMessageBox::warning(0, tr("hidden"),
          tr("Cannot reselect, row is hidden."));
        return;
    }
    _table->scrollTo(index, QAbstractItemView::EnsureVisible);
    _table->selectRow(index.row());
}

/* -------------------------------------------------------------------- */

void MainWindow::editCalButtonClicked()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    unsubmittedFormQuery(tr("Edit"));

    // get selected row number
    int row = _table->selectionModel()->currentIndex().row();

    _editRid = modelData(row, clm_rid);
    if (!_editable.contains(_editRid)) {
        _form->setEnabled(false);
        QMessageBox::warning(0, tr("edit"),
           tr("Cannot edit content, clone it first then edit that line."));
    }
    else
        _form->setEnabled(true);

    // set the form's data widget mapper to the selected row
    _form->setRow(row);

    initializeForm(row);
}

/* -------------------------------------------------------------------- */

void MainWindow::initializeForm(int row)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // populate the form view with elements from this row
    QString rid      = modelData(row, clm_rid);
    QString pid      = modelData(row, clm_pid);
    QString site     = modelData(row, clm_site);
    QString var_name = modelData(row, clm_var_name);
    QString cal_date = modelData(row, clm_cal_date);

    std::cout << "site: " <<  site.toStdString() << std::endl;
    std::cout << "var_name: " <<  var_name.toStdString() << std::endl;
    std::cout << "cal_date: " <<  cal_date.toStdString() << std::endl;

    QSqlQuery query(QSqlDatabase::database());
    QString cmd("SELECT cal FROM "DB_TABLE" WHERE site='" + site +
                "' AND var_name='" + var_name + "' AND cal_date<'" + cal_date +
                "' ORDER BY cal_date DESC LIMIT 1");
    qDebug() << "cmd:" << cmd;

    QString prevString = "{}";
    if (query.exec(cmd) && query.first())
        prevString = query.value(0).toString();
    query.finish();

    form_set_times  = modelData(row, clm_set_times);
    form_set_points = modelData(row, clm_set_points);
    form_averages   = modelData(row, clm_averages);
    form_stddevs    = modelData(row, clm_stddevs);
    form_cal        = modelData(row, clm_cal);

    QStringList list_set_times  = extractListFromBracedCSV(form_set_times);
    QStringList list_set_points = extractListFromBracedCSV(form_set_points);
    QStringList list_averages   = extractListFromBracedCSV(form_averages);
    QStringList list_stddevs    = extractListFromBracedCSV(form_stddevs);
    QStringList list_cal        = extractListFromBracedCSV(form_cal);
    QStringList list_old        = extractListFromBracedCSV( prevString );

    if (pid.length())
        restrictOrderChoice(list_set_points);

    int i = 0;
    std::vector<double> _cals;
    foreach( QString coeff, list_cal) {
        double iCd = coeff.toDouble();
        _cals.push_back(iCd);
        _form->_currCalCList[i++]->setText(coeff);
    }
    QList<QAbstractButton *> curveFitButtons = _form->_curveFitGroup->buttons();
    foreach (QAbstractButton *button, curveFitButtons)
        button->setChecked(false);

    curveFitButtons[i-2]->setChecked(true);

    for (; i<4;)
        _form->_currCalCList[i++]->setText("");

    i = 0;
    foreach( QString coeff, list_old)
        _form->_prevCalCList[i++]->setText(coeff);

    for (; i<4;)
        _form->_prevCalCList[i++]->setText("");

    i = 0;
    QStringListIterator iT(list_set_times);
    QStringListIterator iP(list_set_points);
    QStringListIterator iA(list_averages);
    QStringListIterator iD(list_stddevs);
    while (iP.hasNext()) {

        // skip over removed set points
        QString sp = iP.next();
        if ( sp.isEmpty() ) {
            if (list_set_times.count() > 1)
                iT.next();
            iA.next();
            iD.next();
            _form->_setDateTimeList[i]->setText("");
            _form->_setPointList[i]->setText(   "");
            _form->_newVList[i]->setText(       "");
            _form->_new_sdList[i]->setText(     "");
            _form->_appliedList[i]->setText(    "");
            _form->_delButtonList[i]->setEnabled(false);
            i++;
            continue;
        }
        // mitigate old data that predates the use of set_times
        QString dt;
        if (list_set_times.count() > 1)
            dt = iT.next();

        // calculate applied value
        QString average = iA.next();
        double voltage = average.toDouble();
        QString applied = QString::number(numeric::PolyEval(_cals, voltage));

        _form->_tableWidget->setRowHidden(i, false);

        _form->_setDateTimeList[i]->setText(dt.remove(QChar('"')));
        _form->_setPointList[i]->setText(   sp);
        _form->_newVList[i]->setText(       average);
        _form->_new_sdList[i]->setText(     iD.next());
        _form->_appliedList[i]->setText(    applied);
        _form->_delButtonList[i]->setEnabled(true);
        i++;
    }
    for (; i<nRows; i++)
        _form->_tableWidget->setRowHidden(i, true);

    _form->_tableWidget->resizeColumnsToContents();

    // stuff QLineEdit elements
    _form->       _ridTxt->setText( rid );
    _form->  _platformTxt->setText( site );

    // stuff QComboBox elements
    _form->      _projTxt->setCurrentIndex( _form->      _projTxt->findText( modelData(row, clm_project_name ) ) );
    _form->      _userTxt->setCurrentIndex( _form->      _userTxt->findText( modelData(row, clm_username ) ) );
    _form->_sensorTypeTxt->setCurrentIndex( _form->_sensorTypeTxt->findText( modelData(row, clm_sensor_type ) ) );
    _form-> _serialNumTxt->setCurrentIndex( _form-> _serialNumTxt->findText( modelData(row, clm_serial_number ) ) );
    _form->       _dsmTxt->setCurrentIndex( _form->       _dsmTxt->findText( modelData(row, clm_dsm_name ) ) );
    _form->   _calTypeTxt->setCurrentIndex( _form->   _calTypeTxt->findText( modelData(row, clm_cal_type ) ) );
    _form->      _addrTxt->setCurrentIndex( _form->      _addrTxt->findText( modelData(row, clm_channel ) ) );
    _form->  _gainbplrTxt->setCurrentIndex( _form->  _gainbplrTxt->findText( modelData(row, clm_gainbplr ) ) );
}

/* -------------------------------------------------------------------- */

void MainWindow::submitForm(int row)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    _proxy->setData(_proxy->index(row, clm_set_times),  form_set_times);
    _proxy->setData(_proxy->index(row, clm_set_points), form_set_points);
    _proxy->setData(_proxy->index(row, clm_averages),   form_averages);
    _proxy->setData(_proxy->index(row, clm_stddevs),    form_stddevs);
    _proxy->setData(_proxy->index(row, clm_cal),        form_cal);

    changeDetected = true;
}

/* -------------------------------------------------------------------- */

void MainWindow::plotCalButtonClicked()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // get selected row number
    int row = _table->selectionModel()->currentIndex().row();
    plotCalButtonClicked(row);
}

/* -------------------------------------------------------------------- */

void MainWindow::plotCalButtonClicked(int row)
{
    std::cout << __PRETTY_FUNCTION__ << " row: " << row << std::endl;

    // don't plot if already plotted
    QString rid = modelData(row, clm_rid);
    foreach (CalibrationCurve *curve, plottedCurves)
        if (curve->rid == rid) return;

    QString label = modelData(row, clm_var_name) + " " + modelData(row, clm_cal_date);
    std::cout << "label: " << label.toStdString() << std::endl;

    QwtLegend *legend = new QwtLegend;

#if QWT_VERSION >= 0x060100
    legend->setDefaultItemMode(QwtLegendData::Checkable);
#else
    legend->setItemMode(QwtLegend::CheckableItem);
#endif

    _plot->qwtPlot->insertLegend(legend, QwtPlot::BottomLegend);

    QStringList list_set_points;
    QStringList list_averages;
    QStringList list_cal;

    // if row is currently being edited in the form then plot the unsubmitted
    // data instead on the model data
    if (rid == _editRid) {
        list_set_points = extractListFromBracedCSV(form_set_points);
        list_averages   = extractListFromBracedCSV(form_averages);
        list_cal        = extractListFromBracedCSV(form_cal);
    } else {
        list_set_points = extractListFromBracedCSV(row, clm_set_points);
        list_averages   = extractListFromBracedCSV(row, clm_averages);
        list_cal        = extractListFromBracedCSV(row, clm_cal);
    }
    if (list_set_points.isEmpty()) return;
    if (list_averages.isEmpty()) return;
    if (list_cal.isEmpty()) return;

    // fetch a new color to plot in
    if ( _plot->colors.empty()) {
        QMessageBox::information(0, tr("notice"),
          tr("Ran out of unique colors to plot in!\nCurrently only 12 colors are specified."));
        return;
    }
    QColor color = _plot->colors.front();
    std::cout << "pull color: " << color.red() << " " << color.green() << " " << color.blue() << std::endl;
    _plot->colors.pop_front();

    QColor actColor = color;
    QColor fitColor = color.lighter(150);
    QColor mrkColor = color.darker(150);

    std::cout << "actColor: " << actColor.red() << " " << actColor.green() << " " << actColor.blue() << std::endl;
    std::cout << "fitColor: " << fitColor.red() << " " << fitColor.green() << " " << fitColor.blue() << std::endl;
    std::cout << "mrkColor: " << mrkColor.red() << " " << mrkColor.green() << " " << mrkColor.blue() << std::endl;

    // highlight the row in the table to match the color
    _delegate->highlightRow(rid, actColor);

    std::vector<double> _cals;
    foreach( QString coeff, list_cal)
        _cals.push_back( coeff.toDouble() );

    // Run 80 points from -10 to 10 Vdc.
    int nPoints = 80;
/*
    double step = 20.0 / nPoints, vdc = -10.0;
*/
    QStringList aList;
    if ( modelData(row, clm_cal_type) == "bath")
        aList = list_set_points;
    else
        aList = list_averages;
    QStringListIterator iX(aList);

    float max = -9999.9; 
    float min =  9999.9; 
    std::cout << "min: " << min << " max: " << max << std::endl;
    while (iX.hasNext()) {
        QString iPs = iX.next();
        if (!iPs.isEmpty()) {
            float iPf = iPs.toFloat();
            std::cout << "val: " << iPf << std::endl;
            if (max < iPf) max = iPf;
            if (min > iPf) min = iPf;
        }
    }
    std::cout << "min: " << min << " max: " << max << std::endl;
    double step = (max-min) / nPoints, vdc = min;

    std::vector<double> x, y;
    x.resize(nPoints);
    y.resize(nPoints);

    for (int i = 0; i < nPoints; ++i)
    {
      x[i] = vdc;
      y[i] = numeric::PolyEval(_cals, x[i]);
      vdc += step;
    }
    
    QVector<QPointF>* actual = new QVector<QPointF>;

    QStringListIterator iA(list_averages);
    QStringListIterator iP(list_set_points);
    while (iA.hasNext() && iP.hasNext())
    {
        // skip over removed set points
        QString sp = iP.next();
        if ( sp.isEmpty() ) {
            iA.next();
            continue;
        }
        float iAf = iA.next().toFloat();
        float iPf = sp       .toFloat();
        std::cout << iAf << " " << iPf << std::endl;
        actual->push_back(QPointF(iAf, iPf));
    }
    QwtText actLabel("actual "+label);
    QwtText fitLabel("fitted "+label);

    actLabel.setColor(actColor);
    fitLabel.setColor(fitColor);

    CalibrationCurve* curve = new CalibrationCurve;
    curve->rid = rid;
    curve->actual = new QwtPlotCurve(actLabel);
    curve->fitted = new QwtPlotCurve(fitLabel);

    QPen actPen(actColor);  actPen.setWidth(1);
    curve->actual->setPen(actPen);

    QPen fitPen(fitColor);  fitPen.setWidth(1);
    curve->fitted->setPen(fitPen);

#if QWT_VERSION >= 0x060000
    QwtSymbol *cross = new QwtSymbol(QwtSymbol::Cross, mrkColor, mrkColor, QSize(5, 5));
    curve->actual->setSymbol(cross);
#else
    QwtSymbol cross(QwtSymbol::Cross, mrkColor, mrkColor, QSize(5, 5));
    curve->actual->setSymbol(cross);
#endif

#if QWT_VERSION >= 0x060000
    QwtPointSeriesData* actData = new QwtPointSeriesData;
    actData->setSamples(*actual);

    curve->actual->setData(actData);
    if ( modelData(row, clm_cal_type) == "bath")
        curve->fitted->setSamples(&y[0], &x[0], nPoints);
    else
        curve->fitted->setSamples(&x[0], &y[0], nPoints);
#else
    // TODO: figure out how to implement the above in version 5. Until then, issue a warning.
#warning "Source is not compatible with version 5 of QWT"
#endif

    curve->actual->attach(_plot->qwtPlot);
    curve->fitted->attach(_plot->qwtPlot);
    plottedCurves.append(curve);

//  TODO implement zooming via rubberband selection
//  QRectF region = actData->boundingRect();
    QwtPlotZoomer *zoomer = new QwtPlotZoomer(_plot->qwtPlot->canvas());
    zoomer->zoom(0);
//  _plot->qwtPlot->replot();
}

/* -------------------------------------------------------------------- */

void MainWindow::unplotCalButtonClicked()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // get selected row number
    int row = _table->selectionModel()->currentIndex().row();
    unplotCalButtonClicked(row);
}

/* -------------------------------------------------------------------- */

void MainWindow::unplotAllButtonClicked()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    foreach (CalibrationCurve *curve, plottedCurves) {
        curve->actual->detach();
        curve->fitted->detach();
        plottedCurves.removeOne(curve);
        _delegate->unhighlightRow(curve->rid);
    }
    _plot->dropColors();
    _plot->setupColors();
    _plot->qwtPlot->replot();
}

/* -------------------------------------------------------------------- */

void MainWindow::unplotCalButtonClicked(int row)
{
    std::cout << __PRETTY_FUNCTION__ << " row: " << row << std::endl;

    QString rid = modelData(row, clm_rid);
    std::cout << "selected rid: " << rid.toStdString() << std::endl;

    foreach (CalibrationCurve *curve, plottedCurves) {
        std::cout << "iterated rid: " << curve->rid.toStdString() << std::endl;
        if (curve->rid == rid) {
            QColor color = curve->actual->pen().color();
            std::cout << "push color: " << color.red() << " " << color.green() << " " << color.blue() << std::endl;
            _plot->colors.push_front(color);
            curve->actual->detach();
            curve->fitted->detach();
            plottedCurves.removeOne(curve);
        } 
    } 
    _plot->qwtPlot->replot();
    _delegate->unhighlightRow(rid);
}

/* -------------------------------------------------------------------- */

void MainWindow::exportCalButtonClicked()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // get selected row number
    int row = _table->selectionModel()->currentIndex().row();

    // don't export anything that was removed
    QString removed = modelData(row, clm_removed);
    if (removed == "1") {
        QMessageBox::information(0, tr("notice"),
          tr("You cannot export a calibration from a removed row."));
        return;
    }
    // get the cal_type from the selected row
    QString cal_type = modelData(row, clm_cal_type);
    std::cout << "cal_type: " <<  cal_type.toStdString() << std::endl;

    if (cal_type == "instrument")
        exportInstrument(row);

    if (cal_type == "analog")
        exportAnalog(row);

    if (cal_type == "bath")
        exportBath(row);
}

/* -------------------------------------------------------------------- */

void MainWindow::exportCsvButtonClicked()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // get selected row number
    int row = _table->selectionModel()->currentIndex().row();

    QStringList list_set_points = extractListFromBracedCSV(row, clm_set_points);
    if (list_set_points.isEmpty()) return;
    QStringList list_averages   = extractListFromBracedCSV(row, clm_averages);
    if (list_averages.isEmpty()) return;

    std::ostringstream ostr;
    ostr << "setPoint,Average\n";

    QStringListIterator iP(list_set_points);
    QStringListIterator iA(list_averages);
    while (iP.hasNext() && iA.hasNext())
        ostr << iP.next().toStdString() << ","
             << iA.next().toStdString() << "\n";

    QString site     = modelData(row, clm_site);
    QString var_name = modelData(row, clm_var_name);

    QString filename = csvfile_dir;
    filename += site + "_" + var_name + ".csv";

    saveFileAs(filename, tr("CSV files")+" (*.dat)", ostr.str());
}

/* -------------------------------------------------------------------- */

void MainWindow::viewCalButtonClicked()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // get selected row number
    int row = _table->selectionModel()->currentIndex().row();

    // get the cal_type from the selected row
    QString cal_type = modelData(row, clm_cal_type);

    QString filename = calfile_dir;

    if (cal_type == "instrument") {
        QString var_name = modelData(row, clm_var_name);

        // get the site from the selected row
        QString site = modelData(row, clm_site);

#ifndef SANDBOX
        filename += QString("Engineering/") + site + "/";
#endif
        filename += var_name + ".dat";
    }
    else if (cal_type == "analog") {
        // extract the serial_number of the A2D card from the current row
        QString serial_number = modelData(row, clm_serial_number);

#ifndef SANDBOX
        filename += QString("A2D/");
#endif
        filename += "A2D" + serial_number + ".dat";
    }
    else if (cal_type == "bath") {
        QString project_name  = modelData(row, clm_project_name);
        QString sensor_type   = modelData(row, clm_sensor_type);
        QString serial_number = modelData(row, clm_serial_number);

        filename = csvfile_dir;
        filename += project_name + "_" + sensor_type + "_" + serial_number + ".bath";
    }
    else 
        return;

    viewFile(filename);
}

/* -------------------------------------------------------------------- */

void MainWindow::viewCsvButtonClicked()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // get selected row number
    int row = _table->selectionModel()->currentIndex().row();

    QString site     = modelData(row, clm_site);
    QString var_name = modelData(row, clm_var_name);

    QString filename = csvfile_dir;
    filename += site + "_" + var_name + ".csv";

    viewFile(filename);
}

/* -------------------------------------------------------------------- */

void MainWindow::viewFile(QString filename)
{
    qDebug() << "filename: " <<  filename;
    QFile file(filename);
    if (file.open(QFile::ReadOnly)) {
        QTextStream in(&file);
        const QString data = in.readAll();
        viewText(data, filename);
    }
    else
        QMessageBox::information(0, tr("notice"),
          tr("missing:\n") + filename + tr("\n\nNot found."));
}

/* -------------------------------------------------------------------- */

void MainWindow::viewText(QString text, QString title)
{
    ViewTextDialog viewTextDialog;
    viewTextDialog.setWindowTitle(QApplication::translate("Ui::ViewTextDialog",
      title.toStdString().c_str(), 0, QApplication::UnicodeUTF8));
    viewTextDialog.setContents(&text);
    viewTextDialog.exec();
}

/* -------------------------------------------------------------------- */

void MainWindow::exportInstrument(int row)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    QString var_name = modelData(row, clm_var_name);
    std::cout << "var_name: " <<  var_name.toStdString() << std::endl;

    // extract the site of the instrument from the current row
    QString site = modelData(row, clm_site);
    std::cout << "site: " <<  site.toStdString() << std::endl;

    // extract the cal_date from the current row
    QString cal_date = modelData(row, clm_cal_date);
    QDateTime ct = QDateTime::fromString(cal_date, Qt::ISODate);

    // extract the cal coefficients from the selected row
    QStringList list_cal = extractListFromBracedCSV(row, clm_cal);
    if (list_cal.isEmpty()) return;

    // record results to the device's CalFile
    std::ostringstream ostr;
    ostr << std::endl;

    ostr << ct.toString("yyyy MMM dd HH:mm:ss").toStdString();
    qDebug() << "ct: " << ct.toString("yyyy MMM dd HH:mm:ss");

    foreach (QString coeff, list_cal)
        ostr << " " << std::setw(9) << coeff.toStdString();

    ostr << " # " << QString(getenv("USERNAME")).toStdString();
    ostr << " " << modelData(row, clm_project_name).toStdString();
    ostr << " " << modelData(row, clm_comment).toStdString();
    ostr << std::endl;

    QString filename = calfile_dir;

#ifndef SANDBOX
    filename += QString("Engineering/") + site + "/";
#endif
    filename += var_name + ".dat";

    exportCalFile(filename, ostr.str());
}

/* -------------------------------------------------------------------- */

void MainWindow::exportAnalog(int row)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // extract the serial_number of the A2D card from the current row
    QString serial_number = modelData(row, clm_serial_number);
    std::cout << "serial_number: " <<  serial_number.toStdString() << std::endl;

    QString gainbplr = modelData(row, clm_gainbplr);
    std::cout << "gainbplr: " <<  gainbplr.toStdString() << std::endl;

    // extract the cal_date from the current row
    QString cal_date = modelData(row, clm_cal_date);
    QDateTime ut, ct = QDateTime::fromString(cal_date, Qt::ISODate);

    // extract the cal coefficients from the selected row
    QString offst[8];
    QString slope[8];
    QRegExp rxCoeff2("\\{([+-]?\\d+\\.\\d+),([+-]?\\d+\\.\\d+)\\}");

    QString cal = modelData(row, clm_cal);
    if (rxCoeff2.indexIn(cal) == -1) {
        QMessageBox::information(0, tr("notice"),
          tr("You must select a calibration matching\n\n'") + rxCoeff2.pattern() + 
          tr("'\n\nto export an analog calibration."));
        return;
    }
    int channel = modelData(row, clm_channel).toInt();
    offst[channel] = rxCoeff2.cap(1);
    slope[channel] = rxCoeff2.cap(2);
    int chnMask = 1 << channel;

    // search for the other channels and continue extracting coefficients...
    int topRow = row;
    do {
        if (--topRow < 0)                                          break;
        if           ("1" == modelData(topRow, clm_removed))       continue;
        if (serial_number != modelData(topRow, clm_serial_number)) break;
        if      ("analog" != modelData(topRow, clm_cal_type))      break;
        if      (gainbplr != modelData(topRow, clm_gainbplr))      break;

        cal_date = modelData(topRow, clm_cal_date);
        ut = QDateTime::fromString(cal_date, Qt::ISODate);
        qDebug() << "| " << ut << " - " << ct << " | = "
                 << std::abs(ut.secsTo(ct))
                 << " > " << 12*60*60;
        if (std::abs(ut.secsTo(ct)) > 12*60*60) break;

        cal = modelData(topRow, clm_cal);
        if (rxCoeff2.indexIn(cal) == -1) {
            QMessageBox::information(0, tr("notice"),
              tr("You must select a calibration matching\n\n'") + rxCoeff2.pattern() + 
              tr("'\n\nto export an analog calibration."));
            return;
        }
        channel = modelData(topRow, clm_channel).toInt();
        offst[channel] = rxCoeff2.cap(1);
        slope[channel] = rxCoeff2.cap(2);
        chnMask |= 1 << channel;

        // select the rows of what's found
        _table->selectionModel()->select(_proxy->index(topRow, 0),
            QItemSelectionModel::Select | QItemSelectionModel::Rows);
    } while (true);
    topRow++;

    int numRows = _proxy->rowCount() - 1;
    int btmRow = row;
    do {
        if (++btmRow > numRows)                                    break;
        if           ("1" == modelData(btmRow, clm_removed))       continue;
        if (serial_number != modelData(btmRow, clm_serial_number)) break;
        if      ("analog" != modelData(btmRow, clm_cal_type))      break;
        if      (gainbplr != modelData(btmRow, clm_gainbplr))      break;

        cal_date = modelData(btmRow, clm_cal_date);
        ut = QDateTime::fromString(cal_date, Qt::ISODate);
        qDebug() << "| " << ut << " - " << ct << " | = "
                 << std::abs(ut.secsTo(ct))
                 << " > " << 12*60*60;
        if (std::abs(ut.secsTo(ct)) > 12*60*60) break;

        cal = modelData(btmRow, clm_cal);
        if (rxCoeff2.indexIn(cal) == -1) {
            QMessageBox::information(0, tr("notice"),
              tr("You must select a calibration matching\n\n'") + rxCoeff2.pattern() + 
              tr("'\n\nto export an analog calibration."));
            return;
        }
        channel = modelData(btmRow, clm_channel).toInt();
        offst[channel] = rxCoeff2.cap(1);
        slope[channel] = rxCoeff2.cap(2);
        chnMask |= 1 << channel;

        // select the rows of what's found
        _table->selectionModel()->select(_proxy->index(btmRow, 0),
            QItemSelectionModel::Select | QItemSelectionModel::Rows);
    } while (true);
    btmRow--;

    // complain if the found selection is discontiguous or an undistinct set of 8
    int numFound  = btmRow - topRow + 1;
    std::cout << "chnMask: 0x" << std::hex << chnMask << std::dec << std::endl;
    std::cout << "numFound: " << numFound << std::endl;
    if ((chnMask != 0xff) || (numFound != 8)) {
        QMessageBox::information(0, tr("notice"),
          tr("Discontiguous or an undistinct selection found.\n\n") +
          tr("You need 8 channels selected to generate a calibration dat file!"));
        return;
    }
    // extract temperature from the btmRow
    QString temperature = modelData(btmRow, clm_temperature);

    // extract cal_date from channel 0
    int chn0idx = -1;
    QModelIndexList rowList = _table->selectionModel()->selectedRows();
    foreach (QModelIndex rowIndex, rowList) {
        chn0idx = rowIndex.row();
        if (modelData(chn0idx, clm_channel) == "0")
            break;
    }
    cal_date = modelData(chn0idx, clm_cal_date);
    ut = QDateTime::fromString(cal_date, Qt::ISODate);

    // record results to the device's CalFile
    std::ostringstream ostr;
    ostr << std::endl;
    ostr << "# temperature: " << temperature.toStdString() << std::endl;
    ostr << "#  Date              Gain  Bipolar";
    for (uint ix=0; ix<8; ix++)
        ostr << "  CH" << ix << "-off   CH" << ix << "-slope";
    ostr << std::endl;

    ostr << ut.toString("yyyy MMM dd HH:mm:ss").toStdString();
    qDebug() << "ut: " << ut.toString("yyyy MMM dd HH:mm:ss");

    std::map<QString, std::string> gainbplr_out;
    gainbplr_out["1T"] = "    1       1     ";
    gainbplr_out["2F"] = "    2       0     ";
    gainbplr_out["2T"] = "    2       1     ";
    gainbplr_out["4F"] = "    4       0     ";
    ostr << gainbplr_out[gainbplr];

    for (uint ix=0; ix<8; ix++) {
        ostr << "  " << offst[ix].leftJustified(9).toStdString()
             << " "  << slope[ix].leftJustified(8).toStdString();
    }
    ostr << std::endl;

    QString filename = calfile_dir;

#ifndef SANDBOX
    filename += QString("A2D/");
#endif
    filename += "A2D" + serial_number + ".dat";

    exportCalFile(filename, ostr.str());
}

/* -------------------------------------------------------------------- */

void MainWindow::exportBath(int row)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    QString cal_date      = modelData(row, clm_cal_date);
    QString project_name  = modelData(row, clm_project_name);
    QString sensor_type   = modelData(row, clm_sensor_type);
    QString serial_number = modelData(row, clm_serial_number);

    QDateTime ut = QDateTime::fromString(cal_date, Qt::ISODate);

    QStringList list_set_points = extractListFromBracedCSV(row, clm_set_points);
    if (list_set_points.isEmpty()) return;
    QStringList list_averages   = extractListFromBracedCSV(row, clm_averages);
    if (list_averages.isEmpty()) return;
    QStringList list_stddevs    = extractListFromBracedCSV(row, clm_stddevs);
    if (list_stddevs.isEmpty()) return;

    int nSetPoints = list_set_points.count();
    double setp[nSetPoints];
    double avrg[nSetPoints];

    for (int i=0; i<nSetPoints; i++) {
        setp[i] = list_set_points[i].toDouble();
        avrg[i] = list_averages[i].toDouble();
    }
    // generate a quadratic fit to start with
    double coeff[MAX_ORDER], Rsq;
    polynomialfit(nSetPoints, 3, setp, avrg, coeff, &Rsq);


    std::ostringstream ostr;
    ostr << "NCAR/RAF AIRCRAFT INSTRUMENTATION EQUIPMENT CALIBRATION\n\n";
    ostr << project_name.toStdString() << "      ";
    ostr << ut.toString("yyyy MMM dd HH:mm:ss").toStdString() << "      ";
    ostr << sensor_type.toStdString() << " ";
    ostr << serial_number.toStdString() << "\n\n";

    ostr << "y = " << coeff[2] << "*x^2 ";
    ostr << (coeff[1] < 0 ? "- ":"+ ") << fabs(coeff[1]) << "*x ";
    ostr << (coeff[0] < 0 ? "- ":"+ ") << fabs(coeff[0]);
    ostr << "      R^2: " << Rsq << "\n\n";

    QStringListIterator iP(list_set_points);
    QStringListIterator iA(list_averages);
    QStringListIterator iD(list_stddevs);

    ostr << "Note that the DECADE RESISTANCE SETTING already accounts\n";
    ostr << "for the " << DECADE_BOX_WIRE_RESISTANCE << " Ohm line resistance.\n";
    ostr << "\nCALIBRATION VALUES:\n";
    ostr << "TEMPERATURE   DECADE RESISTANCE   THERMOMETER   MEASUREMENT\n";
    ostr << "  DEG C.        SETTING OHMS       STD DEV      ACCEPTABLE?\n";
//  ostr << "  XXXxXXX         ZZzZZZ           DDdDDDDDD      _________\n";
    ostr << "-----------------------------------------------------------\n";

    while (iP.hasNext() && iA.hasNext()) {
        double x = iP.next().toDouble();
        double y = iA.next().toDouble();
        double d = iD.next().toDouble();
        QString line;
        line.sprintf("\n  %7.3f         %6.3f           %9.6f"
                     "     _________\n",
                     x, y - DECADE_BOX_WIRE_RESISTANCE, d);
        ostr << line.toStdString();
    }
    QString filename = csvfile_dir;
    filename += project_name + "_" + sensor_type + "_" + serial_number + ".bath";

    saveFileAs(filename, tr("bath files")+" (*.bath)", ostr.str());

    // mark what's exported
    _proxy->setData(_proxy->index(row, clm_exported), "1");
}

/* -------------------------------------------------------------------- */

void MainWindow::exportCalFile(QString filename, std::string contents)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    QMessageBox::information(0, tr("Exporting cal into file:"), filename);
    qDebug() << "filename: " << filename;
    qDebug() << "contents: " << contents.c_str();

    // this matches date and time stamps like this: 2008 Jun 09 19:47:05
    QRegExp rxDateTime("^([12][0-9][0-9][0-9] ... [ 0-3][0-9] "
                       "[ 0-2][0-9]:[0-5][0-9]:[0-5][0-9])");

    QDateTime bt, ct, et(QDate(2999,1,1),QTime(0,0,0));

    // Find the actual singular calibration line in the new contents.
    QString calLine, qstr(contents.c_str());
    QStringList qsl = qstr.split("\n");
    foreach (calLine, qsl) {
        qDebug() << "calLine: " << calLine;
        if ( calLine.contains(rxDateTime) )
            break;
    }
    if ( not calLine.contains(rxDateTime) )
        return;

    ct = QDateTime::fromString(rxDateTime.cap(1), "yyyy MMM dd HH:mm:ss");
    qDebug() << "ct: " << ct.toString();

    // Open the selected calfile (it's ok if it doesn't exist yet).
    std::ifstream calfile ( filename.toStdString().c_str() );

    // Open a unique temporary file.
    char tmpfilename[] = "/tmp/MainWindow_XXXXXX";
    int tmpfile = mkstemp(tmpfilename);
    if (tmpfile == -1) {
        if (calfile) calfile.close();
        std::ostringstream ostr;
        ostr << tr("failed to open temporary file:\n").toStdString();
        ostr << tmpfilename << std::endl;
        ostr << tr(strerror(errno)).toStdString();
        QMessageBox::warning(0, tr("error"), ostr.str().c_str());
        return;
    }
    // Read the selected calfile and look for a place, chronologically, to insert
    // the new calibration entry.
    std::ostringstream buffer;
    std::string line;
    while ( std::getline(calfile, line) ) {
        buffer << line << std::endl;
//      std::cout << "-- buffer -------------------------------------------------------------\n"
//                << buffer.str().c_str()
//                << "-- ------ -------------------------------------------------------------\n";

        // If line matches a previously inserted line then prompt before re-inserting it.
        if ( QString(line.c_str()) == calLine) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(0, tr("Duplicate?"),
                        tr("Calibration was already exported to this file\n\nCreate a duplicate entry in file?\n"),
                        QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::No)
                ct = et;
        }
        // If successfully inserted then dump remainder of calfile into tmpfile.
        if ( ct == et) {
//          std::cout << "APPENDING REMAINDER | " << buffer.str().c_str();
            write(tmpfile, buffer.str().c_str(), buffer.str().length());
            buffer.str("");
            continue;
        }
        // Find datetime stamp from the latest line in the buffer.
        if (rxDateTime.indexIn( QString(line.c_str()) ) == -1)
            continue;

        // insert leading zeros to day and hour fields
        // 01234567890123456789
        // yyyy MMM dd HH:mm:ss
        QString aTime = rxDateTime.cap(1);
        if (aTime[9]  == ' ') aTime[9]  = '0';
        if (aTime[12] == ' ') aTime[12] = '0';
        bt = QDateTime::fromString(aTime, "yyyy MMM dd HH:mm:ss");
//      std::cout << "  bt: " << bt.toString().toStdString() << " (" << rxDateTime.cap(1).toStdString() << ")" << std::endl;
        if ( ct < bt) {
            ct = et;
            write(tmpfile, contents.c_str(), contents.length());
        }
        write(tmpfile, buffer.str().c_str(), buffer.str().length());
        buffer.str("");
    }
    // Insert the contents if not already done yet.
    if ( ct != et)
        write(tmpfile, contents.c_str(), contents.length());

    if (calfile) calfile.close();
    ::close(tmpfile);

    chmod(tmpfilename, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);

    // Replace calfile with tmpfile.
    QFile::remove(filename);
    if ( !QFile::rename(QString(tmpfilename), filename) ) {
        std::ostringstream ostr;
        ostr << tr("failed to create cal file:\n").toStdString();
        ostr << filename.toStdString() << std::endl;
        ostr << tr(strerror(errno)).toStdString();
        QMessageBox::warning(0, tr("error"), ostr.str().c_str());
        return;
    }
    // mark what's exported
    QModelIndexList rowList = _table->selectionModel()->selectedRows();
    foreach (QModelIndex rowIndex, rowList) {
        _proxy->setData(_proxy->index(rowIndex.row(), clm_exported), "1");
    }
}

/* -------------------------------------------------------------------- */

void MainWindow::saveFileAs(QString filename, QString title, std::string contents)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
    // provide a dialog for the user to select what to save as...
    filename = QFileDialog::getSaveFileName(this, tr("Save File"),
                 filename, title);
                 
    if (filename.isEmpty()) return;
    
    std::cout << "Writing results to: ";
    std::cout << filename.toStdString() << std::endl;
    std::cout << contents << std::endl; 
    
    // TODO - why isn't older files being completely replaced here?
    // HACK remove previous file first
    QFile::remove(filename);
    int fd = ::open( filename.toStdString().c_str(),
                     O_WRONLY | O_CREAT, 0664);
                     
    if (fd == -1) {  
        std::ostringstream ostr;
        ostr << tr("failed to save results to:\n").toStdString();
        ostr << filename.toStdString() << std::endl;
        ostr << tr(strerror(errno)).toStdString();
        QMessageBox::warning(0, tr("error"), ostr.str().c_str());
        return;
    }   
    write(fd, contents.c_str(), contents.length());
    ::close(fd);
}   

/* -------------------------------------------------------------------- */

void MainWindow::cloneButtonClicked()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    QModelIndex index = _table->selectionModel()->currentIndex();
    int row = index.row();
    std::cout << "row = " << row << std::endl;

    // set clone's row ID
    QString rid           = createRID();

    // set clone's parent ID
    QString pid           = modelData(row, clm_rid);

    // copy data from parent row
    QString site          = modelData(row, clm_site);
    QString pulled        = "0";
    QString removed       = "0";
    QString exported      = "0";
    QDateTime cal_date    = _proxy->index(row, clm_cal_date).data().toDateTime();
    QString project_name  = modelData(row, clm_project_name);
    QString username      = getenv("USERNAME");
    QString sensor_type   = modelData(row, clm_sensor_type);
    QString serial_number = modelData(row, clm_serial_number);
    QString var_name      = modelData(row, clm_var_name);
    QString dsm_name      = modelData(row, clm_dsm_name);
    QString cal_type      = modelData(row, clm_cal_type);
    QString channel       = modelData(row, clm_channel);
    QString gainbplr      = modelData(row, clm_gainbplr);
    QString ads_file_name = modelData(row, clm_ads_file_name);
    QString set_times     = modelData(row, clm_set_times);
    QString set_points    = modelData(row, clm_set_points);
    QString averages      = modelData(row, clm_averages);
    QString stddevs       = modelData(row, clm_stddevs);
    QString cal           = modelData(row, clm_cal);
    QString temperature   = modelData(row, clm_temperature);
    QString comment       = modelData(row, clm_comment);

    // add new rid to list of editable clones
    _editable.push_back(rid);

    // advance the clone's timestamp to be one second past the parent's
    cal_date = cal_date.addSecs(1);

    QSqlRecord record = _model->record();

    // paste the parent's data into its clone
    record.setValue(clm_rid,           rid);
    record.setValue(clm_pid,           pid);
    record.setValue(clm_site,          site);
    record.setValue(clm_pulled,        pulled);
    record.setValue(clm_removed,       removed);
    record.setValue(clm_exported,      exported);
    record.setValue(clm_cal_date,      cal_date);
    record.setValue(clm_project_name,  project_name);
    record.setValue(clm_username,      username);
    record.setValue(clm_sensor_type,   sensor_type);
    record.setValue(clm_serial_number, serial_number);
    record.setValue(clm_var_name,      var_name);
    record.setValue(clm_dsm_name,      dsm_name);
    record.setValue(clm_cal_type,      cal_type);
    record.setValue(clm_channel,       channel);
    record.setValue(clm_gainbplr,      gainbplr);
    record.setValue(clm_ads_file_name, ads_file_name);
    record.setValue(clm_set_times,     set_times);
    record.setValue(clm_set_points,    set_points);
    record.setValue(clm_averages,      averages);
    record.setValue(clm_stddevs,       stddevs);
    record.setValue(clm_cal,           cal);
    record.setValue(clm_temperature,   temperature);
    record.setValue(clm_comment,       comment);

    _model->insertRow(row+1);
    _model->setRecord(row+1, record);

    // re-apply any filtering after inserting a new row
    _lastRid = rid;
    qDebug() << "_lastRid:" << _lastRid;
    _proxy->refreshFilters();
    scrollToLastClicked();

    // Adding a new row does not trigger a dataChanged event
    changeDetected = true;
}

/* -------------------------------------------------------------------- */

void MainWindow::removeButtonClicked()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    // get selected row number
    int row = _table->selectionModel()->currentIndex().row();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(0, tr("Delete"),
                                  tr("Remove selected row?"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) return;

    // mark what's removed
    _proxy->setData(_proxy->index(row, clm_removed), "1");
}

/* -------------------------------------------------------------------- */

void MainWindow::changeFitButtonClicked(int row, int order)
{
    std::cout << __PRETTY_FUNCTION__ << " row: " << row << " order: " << order << std::endl;

    QStringList list_averages   = extractListFromBracedCSV(form_averages);
    if (list_averages.isEmpty()) return;
    QStringList list_set_points = extractListFromBracedCSV(form_set_points);
    if (list_set_points.isEmpty()) return;

    // exit if array sizes don't match
    if (list_set_points.count() != list_averages.count()) {
        QMessageBox::warning(0, tr("error"),
          tr("sizes of 'averages' and 'set_points' arrays don't match!"));
        return;
    }

    int nSetPoints = list_set_points.count();
    double setp[nSetPoints];
    double avrg[nSetPoints];

    for (int i=0; i<nSetPoints; i++) {
        setp[i] = list_set_points[i].toDouble();
        avrg[i] = list_averages[i].toDouble();
    }
    double coeff[MAX_ORDER], Rsq;

    if ( modelData(row, clm_cal_type) == "bath")
        polynomialfit(nSetPoints, order+1, setp, avrg, coeff, &Rsq);
    else
        polynomialfit(nSetPoints, order+1, avrg, setp, coeff, &Rsq);

    std::stringstream cals;
    cals << "{";

    // change cal data in the form
    int i = 0;
    for (; i < order+1; i++) {
        _form->_currCalCList[i]->setText( QString::number(coeff[i]) );
        cals << coeff[i];
        if (i < order)
            cals << ",";
    }
    for (; i<4; i++)
        _form->_currCalCList[i]->setText("");

    cals << "}";
    form_cal = QString(cals.str().c_str());

    std::cout << "old cal: " << modelData(row, clm_cal).toStdString() << std::endl;
    std::cout << "new cal: " << form_cal.toStdString() << std::endl;
}
