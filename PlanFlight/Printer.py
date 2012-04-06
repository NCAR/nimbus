# file Printer.py
"This was downloaded from somewhere, modified, and used to get printing to work."
# might have to track this down and check on copyright and credit
import wx
from wx import Printout, PrintData, PAPER_LETTER, PrintDialogData
from wx import Printer as wxPrinter, MessageBox, PrintPreview, PrintDialog

def GetErrorText():
    " error logic needed here."
    return "Some error occurred."

class Printer(Printout):
    def __init__(self, frame):
        "Prepares the Printing object.  Note: change current_y for 1, 1.5, 2 spacing for lines."
        Printout.__init__(self)
        self.printer_config = PrintData()
        self.printer_config.SetPaperId(PAPER_LETTER)
        self.frame = frame
        self.doc_text = ''
        self.doc_name = ''
        self.current_y = 15  #y should be either (15, 22, 30)
        if self.current_y == 15:
            self.num_lines_per_page = 50
        elif self.current_y == 22:
            self.num_lines_per_page = 35
        else:
            self.num_lines_per_page = 25


    def Print(self, text, doc_name):
        "Prints the given text.  Currently doc_name logic doesn't exist.  E.g. might be useful for a footer.."
        self.doc_text = text
        self.doc_name = doc_name
        pdd = PrintDialogData()
        pdd.SetPrintData(self.printer_config)
        printer = wxPrinter(pdd)
        if not printer.Print(self.frame,self):
            MessageBox("Unable to print the document.")
        else:
            self.printer_config = wx.PrintData(printer.GetPrintDialogData().GetPrintData())

    def PreviewText(self, text, doc_name):
        "This function displays the preview window for the text with the given header."
        try:
            self.doc_name = doc_name
            self.doc_text = text

            #Destructor fix by Peter Milliken --
            print1 = Printer(self.frame, text = self.doc_text)
            print2 = Printer(self.frame, text = self.doc_text)
            preview = PrintPreview(print1, print2, self.printer_config)
            #preview = PrintPreview(self,self,self.printer_config)
            if not preview.Ok():
                MessageBox("Unable to display preview of document.")
                return

            preview_window = PreviewFrame(preview, self.frame, \
                                            "Print Preview - %s" % doc_name)
            preview_window.Initialize()
            preview_window.SetPosition(self.frame.GetPosition())
            preview_window.SetSize(self.frame.GetSize())
            preview_window.MakeModal(True)
            preview_window.Show(True)
        except:
            MessageBox(GetErrorText())

    def PageSetup(self):
        "This function handles displaying the Page Setup window and retrieving the user selected options."
        config_dialog = wxPrintDialog(self.frame)
        config_dialog.GetPrintDialogData().SetPrintData(self.printer_config)
        config_dialog.GetPrintDialogData().SetSetupDialog(True)
        config_dialog.ShowModal()
        self.printer_config = config_dialog.GetPrintDialogData().GetPrintData()
        config_dialog.Destroy()

    def OnBeginDocument(self,start,end):
        "Do any end of document logic here."
        self.base_OnBeginDocument(start,end)

    def OnEndDocument(self):
        "Do any end of document logic here."
        self.base_OnEndDocument()

    def OnBeginPrinting(self):
        "Do printing initialization logic here."
        self.base_OnBeginPrinting()

    def OnEndPrinting(self):
        "Do any post printing logic here."
        self.base_OnEndPrinting()

    def OnPreparePrinting(self):
        "Do any logic to prepare for printing here."
        self.base_OnPreparePrinting()

    def HasPage(self, page_num):
        "This function is called to determine if the specified page exists."
        return len(self.GetPageText(page_num)) > 0

    def GetPageInfo(self):
        """
        This returns the page information: what is the page range available, and what is the selected page range.
        Currently the selected page range is always the available page range.  This logic should be changed if you need
        greater flexibility.
        """

        minPage = 1
        maxPage = int(len(self.doc_text.split('\n'))/self.num_lines_per_page) + 1
        fromPage, toPage = minPage, maxPage
        return (minPage,maxPage,fromPage,toPage)

    def OnPrintPage(self, page_num):
        "This function / event is executed for each page that needs to be printed."
        dc = self.GetDC()
        x,y = 25, self.current_y
#       if not self.IsPreview():
#           y *=4
        line_count = 1
        for line in self.GetPageText(page_num):
            dc.DrawText(line, x, y*line_count+35)
            line_count += 1

        return True

    def GetPageText(self, page_num):
        "This function returns the text to be displayed for the given page number."
        lines = self.doc_text.split('\n')
#       lines_for_page = lines[(page_num -1)*self.num_lines_per_page: page_num*(self.num_lines_per_page-1)]
        lines_for_page = lines[(page_num -1)*self.num_lines_per_page: page_num*(self.num_lines_per_page)]
        return lines_for_page
