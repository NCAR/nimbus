/*
-------------------------------------------------------------------------
OBJECT NAME:	CanvasWindow.cc

FULL NAME:	

ENTRY POINTS:	CanvasWindow()

STATIC FNS:	

DESCRIPTION:	

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 2003
-------------------------------------------------------------------------
*/

#include "CanvasWindow.h"

#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qmessagebox.h>
#include <qregexp.h>

extern ADS_rtFile	*file;

/* -------------------------------------------------------------------- */
class PrintFilter: public QwtPlotPrintFilter
{
public:
	PrintFilter() {};

  virtual QFont font(const QFont &f, Item, int) const
  {
    QFont f2 = f;
    f2.setPointSize((int)(f.pointSize() * 0.75));
    return f2;
  }
};

/* -------------------------------------------------------------------- */
CanvasWindow::CanvasWindow(QApplication *qApp) : QMainWindow(0, "canvas")
{
  QToolBar *toolBar = new QToolBar(this, "options");
  toolBar->setLabel( "File Operations" );


  QToolButton *q = new QToolButton(toolBar);
  q->setTextLabel("Quit");
  q->setUsesTextLabel(true);
  q->setUsesBigPixmap(false);
  connect(q, SIGNAL(clicked()), qApp, SLOT(quit()));

  QToolButton *p = new QToolButton(toolBar);
  p->setTextLabel("Print");
  p->setUsesTextLabel(true);
  p->setUsesBigPixmap(false);
  connect(p, SIGNAL(clicked()), SLOT(Print()));

  QToolButton *f = new QToolButton(toolBar);
  f->setTextLabel("Freeze");
  f->setUsesTextLabel(true);
  f->setUsesBigPixmap(false);
  connect(f, SIGNAL(clicked()), SLOT(ToggleFreeze()));

  QToolButton *c = new QToolButton(toolBar);
  c->setTextLabel("Counts");
  c->setUsesTextLabel(true);
  c->setUsesBigPixmap(false);
  connect(c, SIGNAL(clicked()), SLOT(SetCounts()));

  QToolButton *v = new QToolButton(toolBar);
  v->setTextLabel("Volts");
  v->setUsesTextLabel(true);
  v->setUsesBigPixmap(false);
  connect(v, SIGNAL(clicked()), SLOT(SetVolts()));

  QToolButton *e = new QToolButton(toolBar);
  e->setTextLabel("Engineering");
  e->setUsesTextLabel(true);
  e->setUsesBigPixmap(false);
  connect(e, SIGNAL(clicked()), SLOT(SetEngineering()));


  QHBox *box = new QHBox(this);
  setCentralWidget((QWidget *)box);

  plot = new DataPlot(box, file);
  list = new QListBox(box);
  list->setFixedWidth(125);
  list->setPaletteBackgroundColor(QColor("black"));
  list->setPaletteForegroundColor(QColor("green"));
  connect(list, SIGNAL(clicked(QListBoxItem *)), SLOT(ModifyActiveVars(QListBoxItem *)));


  for (int i = 0; i < nsdi; ++i)
    list->insertItem(sdi[i]->name);

  for (int i = 0; i < nraw; ++i)
    list->insertItem(raw[i]->name);

  plot->show();
  list->show();

  plot->setUnits(ENGINEERING);

  statusBar()->message( "Ready", 2000 );

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void CanvasWindow::Print()
{
  QPrinter printer;

  QString docName = plot->title();
  if ( docName.isEmpty() )
  {
    docName.replace(QRegExp (QString::fromLatin1 ("\n")), tr (" -- "));
    printer.setDocName(docName);
  }

  printer.setCreator("rtplot");
  printer.setOrientation(QPrinter::Landscape);
  printer.setPageSize(QPrinter::Letter);
  printer.setPrintProgram("lpr");

  if (printer.setup())
    plot->print(printer);

}	/* END PRINT */

/* -------------------------------------------------------------------- */
void CanvasWindow::ToggleFreeze()
{
  plot->ToggleFreeze();

}

/* -------------------------------------------------------------------- */
void CanvasWindow::SetCounts()
{
  plot->setUnits(COUNTS);

}	/* END TOGGLEUNITS */

/* -------------------------------------------------------------------- */
void CanvasWindow::SetVolts()
{
  plot->setUnits(VOLTS);

}	/* END TOGGLEUNITS */

/* -------------------------------------------------------------------- */
void CanvasWindow::SetEngineering()
{
  plot->setUnits(ENGINEERING);

}	/* END TOGGLEUNITS */

/* -------------------------------------------------------------------- */
void AddVariable(int indx, DataPlot *), DeleteVariable(int indx, DataPlot *);

void CanvasWindow::ModifyActiveVars(QListBoxItem *item)
{
  if (item == 0)
    return;

  int	i;
  int	indx = list->currentItem();

  if (indx >= nsdi)
  {
    indx -= nsdi;

    for (i = 0; i < nVariables; ++i)
      if (strcmp(Variable[i].name, raw[indx]->name) == 0)
      {
      	DeleteVariable(i, plot);
      	return;
      }

    if (NumberDataSets + 1 > MAXDATASETS)
    {
      QMessageBox::critical(this, "rtplot", "Out of data sets.", "Cancel",0,0,0,1);
      return;
    }
    else
      AddVariable(indx + nsdi, plot);
  }
  else
  {
    for (i = 0; i < nVariables; ++i)
      if (strcmp(Variable[i].name, sdi[indx]->name) == 0)
      {
        DeleteVariable(i, plot);
        return;
      }

    if (NumberDataSets + 1 > MAXDATASETS)
    {
      QMessageBox::critical(this, "rtplot", "Out of data sets.", "Cancel",0,0,0,1);
      return;
    }
    else
      AddVariable(indx, plot);
  }

  list->clearSelection();
  plot->replot();

}	/* END MODIFYACTIVEVARS */

/* END CANVASWINDOW.CC */
