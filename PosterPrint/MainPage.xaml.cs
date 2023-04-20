using System;
using System.Collections.Generic;
using Windows.Storage.FileProperties;
using Windows.Storage.Pickers;
using Windows.Storage.Streams;
using Windows.Storage;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Navigation;
using Windows.UI.Xaml.Media.Imaging;
using Windows.Graphics.Printing;
using Windows.UI.Xaml.Printing;
using Windows.ApplicationModel.Core;
using Windows.UI.Core;
using Windows.UI.Xaml;
using System.Threading.Tasks;


// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace PosterPrint
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    /// 
    
    public sealed partial class MainPage : Page
    {
        double _actualImageWidth;
        //double _actualImageHeight;
        PrintMediaSize _printMediaSize;
        PrintOrientation _printOrientation;
        //PrintPageDescription _pageDescription;
        private readonly WriteableBitmap _imageData = null;
        private readonly DataModel _options = new DataModel();

        private bool _printCompleted = false;
        private PrintManager printMan;
        private PrintDocument printDoc;
        private IPrintDocumentSource printDocSource;

        public MainPage()
        {
            this.InitializeComponent();
            this.DataContext = _options;
            PictureAria.SizeChanged += ImageAria_SizeChanged;
           
            var di = Windows.Graphics.Display.DisplayInformation.GetForCurrentView();
            var ldpi = di.LogicalDpi;
        }

        private void ImageAria_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            Picture.Width = PictureAria.ActualWidth;
            _actualImageWidth = Picture.Width;
            //MainImage.Height = ImageAria.Height;
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            // Register for PrintTaskRequested event
            printMan = PrintManager.GetForCurrentView();
            printMan.PrintTaskRequested += PrintTaskRequested;

            //// Build a PrintDocument and register for callbacks
            printDoc = new PrintDocument();
            printDocSource = printDoc.DocumentSource;
            printDoc.Paginate += Paginate;
            printDoc.GetPreviewPage += GetPreviewPage;
            printDoc.AddPages += AddPages;
         }

        private void PrintTaskRequested(PrintManager sender, PrintTaskRequestedEventArgs args)
        {
            // Create the PrintTask.
            // Defines the title and delegate for PrintTaskSourceRequested
            var printTask = args.Request.CreatePrintTask("Print", PrintTaskSourceRequrested);
            
            IList<string> displayedOptions = printTask.Options.DisplayedOptions;
            //printTask.Options.PrintQuality = PrintQuality.Normal;
            // Choose the printer options to be shown.
            // The order in which the options are appended determines the order in which they appear in the UI
            //displayedOptions.Clear();
            //displayedOptions.Add(Windows.Graphics.Printing.StandardPrintTaskOptions.Copies);
            //displayedOptions.Add(Windows.Graphics.Printing.StandardPrintTaskOptions.Orientation);
            displayedOptions.Add(Windows.Graphics.Printing.StandardPrintTaskOptions.MediaSize);
            //displayedOptions.Add(Windows.Graphics.Printing.StandardPrintTaskOptions.Collation);
            //displayedOptions.Add(Windows.Graphics.Printing.StandardPrintTaskOptions.Duplex);

            // Handle PrintTask.Completed to catch failed print jobs
            printTask.Completed += PrintTaskCompleted;
        }
        private void Paginate(object sender, PaginateEventArgs e)
        {
            // Get the page description to deterimine how big the page is
            PrintPageDescription pageDescription = e.PrintTaskOptions.GetPageDescription(0);
            if ((_printMediaSize != e.PrintTaskOptions.MediaSize) )
            {
                _printMediaSize = e.PrintTaskOptions.MediaSize;
                Picture.Width = pageDescription.ImageableRect.Width;
                //MainImage.Height = _pageDescription.ImageableRect.Height;
                printDoc.InvalidatePreview();
            }

            if (_printOrientation != e.PrintTaskOptions.Orientation)
            {
                _printOrientation = e.PrintTaskOptions.Orientation;
                Picture.Width = pageDescription.ImageableRect.Width;
                //MainImage.Height = _pageDescription.ImageableRect.Height;
                printDoc.InvalidatePreview();
            }
            // As I only want to print one Rectangle, so I set the count to 1
            printDoc.SetPreviewPageCount(1, PreviewPageCountType.Final);
           
        }

        private void ResizeImageForPrint(PrintPageDescription _pageDescription)
        {
            if (Picture.Width < Picture.Height) 
            { }

        }
        private void GetPreviewPage(object sender, GetPreviewPageEventArgs e)
        {
            // Provide a UIElement as the print preview.
            try
            {
               printDoc.SetPreviewPage(e.PageNumber, this.Picture);
               
             
            }
            catch(System.ArgumentException ex) {}
        }

        private void ResizeImage()
        {
            //MainImage.Width = 200;
            //MainImage.Height = 200;
        }


        private void AddPages(object sender, AddPagesEventArgs e)
        {
            //ResizeImage();
            printDoc.AddPage(this.Picture);
            // Indicate that all of the print pages have been provided
            printDoc.AddPagesComplete();

            
        }

        private void PrintTaskSourceRequrested(PrintTaskSourceRequestedArgs args)
        {
            // Set the document source.
            args.SetSource(printDocSource);
        }
        private async void PrintTaskCompleted(PrintTask sender, PrintTaskCompletedEventArgs args)
        {
            // Notify the user when the print operation fails.
            if (args.Completion == PrintTaskCompletion.Failed)
            {
                await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, async () =>
                {
                    ContentDialog noPrintingDialog = new ContentDialog()
                    {
                        Title = "Printing error",
                        Content = "\nSorry, failed to print.",
                        PrimaryButtonText = "OK"
                    };
                    await noPrintingDialog.ShowAsync();
                });
            }
            _printCompleted = true;
          
        }

        public async void ButtonPrintTapped(object sender, TappedRoutedEventArgs e)
        {
            if (PrintManager.IsSupported())
            {
                try
                {
                    // Show print UI
                    bool v = await PrintManager.ShowPrintUIAsync();
                    if (v)
                    {
                        RestoreImagesize();
                    }
                    
                 }
                catch
                {
                    // Printing cannot proceed at this time
                    ContentDialog noPrintingDialog = new ContentDialog()
                    {
                        Title = "Printing error",
                        Content = "\nSorry, printing can' t proceed at this time.",
                        PrimaryButtonText = "OK"
                    };
                    await noPrintingDialog.ShowAsync();
                }
            }
            else
            {
                // Printing is not supported on this device
                ContentDialog noPrintingDialog = new ContentDialog()
                {
                    Title = "Printing not supported",
                    Content = "\nSorry, printing is not supported on this device.",
                    PrimaryButtonText = "OK"
                };
                await noPrintingDialog.ShowAsync();
            }
           
        }

        private async void RestoreImagesize()
        {
            _printCompleted = false; ;
            do        // В цикле с интервалом 100 миллисекунд проверяем статус процесса
                      //поиска пока он не закончится
            {
                await Task.Delay(TimeSpan.FromMilliseconds(100));
            }
            while (!_printCompleted);
            Picture.Width = _actualImageWidth;
            //MainImage.Height = _actualImageHeight;

         }
        private async void ButtonFileOpen_Tapped(object sender, TappedRoutedEventArgs e)
        {
            FileOpenPicker openPicker = new FileOpenPicker
            {
                SuggestedStartLocation = PickerLocationId.PicturesLibrary,
            };
            openPicker.FileTypeFilter.Add(".jpg");
            openPicker.FileTypeFilter.Add(".bmp");

            StorageFile file = await openPicker.PickSingleFileAsync();

            if (file != null)
            {
                _options.ImageFileName = file.Name;
                _options.ImageFilePath = file.Path.Replace(_options.ImageFileName, "");
                _options.ImageFileNameExtention = file.FileType;

                ImageProperties props = await file.Properties.GetImagePropertiesAsync();

                WriteableBitmap _imageData = new WriteableBitmap((int)props.Width, (int)props.Height);
                
                IRandomAccessStream stream = await file.OpenAsync(FileAccessMode.Read);
                _imageData.SetSource(stream);
                Picture.Source= _imageData;
                
                Picture.Width = PictureAria.ActualWidth;
                //MainImage.Height = ImageAria.ActualHeight;

                _actualImageWidth = Picture.Width;
                //_actualImageHeight = MainImage.Height;
            }
            else
            {
                _options.ImageFileName = "File not loaded.";
            }
        }
    }
}
