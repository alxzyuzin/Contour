/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 * e-mail alx.zyuzin@gmail.com
 * This file is part of Contour project.
 *
 * This class presents main application window
 *
 ---------------------------------------------------------------------------------*/

using ContourExtractorWindowsRuntimeComponent;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.ApplicationModel.Core;
using Windows.Foundation;
using Windows.Graphics.Imaging;
using Windows.Graphics.Printing;
using Windows.Storage;
using Windows.Storage.FileProperties;
using Windows.Storage.Pickers;
using Windows.Storage.Streams;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI.Xaml.Navigation;
using Windows.UI.Xaml.Printing;


namespace ContourUI
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page //, INotifyPropertyChanged
    {
        private readonly GeneralOptions Options = new GeneralOptions();
        readonly AppStatus ApplicationStatus = new AppStatus();
        private ContourBitmap bitmap = null;
        private double _pictureWidthToHeightRatio = 1;

        private string _imageFileNameExtention;
        private string _imageFilePath;


        PrintMediaSize _printMediaSize;
        PrintOrientation _printOrientation;
    
        private PrintManager printMan;
        private PrintDocument printDoc;
        private IPrintDocumentSource printDocSource;

        public MainPage()
        {
            InitializeComponent();
            OptionsWindow.DataContext = Options;
            Options.Restore();
            gridMain.DataContext = ApplicationStatus;
            ApplicationStatus.PropertyChanged += ApplicationStatus_PropertyChanged;
            ApplicationStatus.ProgressValue = 0;
            ApplicationStatus.ProgressBarVisibility = Visibility.Collapsed;
            ApplicationStatus.NumberOfLevels = Options.NumberOfColors;
            ApplicationStatus.ConversionMode = Options.ConversionTypeName + ".";
            Palette.PropertyChanged += Palette_PropertyChanged;
            PictureArea.SizeChanged += PictureArea_SizeChanged;
        }

        private void PictureArea_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            AjustPictureSizeToPictureArea();
        }

        private void AjustPictureSizeToPictureArea()
        {
            if (bitmap != null)
            {
                if ((bitmap.Width - PictureArea.ActualWidth) >= (bitmap.Height - PictureArea.ActualHeight))
                {
                    Picture.Width = PictureArea.ActualWidth;
                }
                else
                {
                    Picture.Height = PictureArea.ActualHeight;
                }
                Picture.HorizontalAlignment = HorizontalAlignment.Center;
                Picture.VerticalAlignment = VerticalAlignment.Center;
            }
        }
        #region Register for printing

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

            var displayedOptions = printTask.Options.DisplayedOptions;
            //printTask.Options.PrintQuality = PrintQuality.Normal;
            // Choose the printer options to be shown.
            // The order in which the options are appended determines the order in which they appear in the UI
            displayedOptions.Clear();
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
            if ((_printMediaSize != e.PrintTaskOptions.MediaSize))
            {
                _printMediaSize = e.PrintTaskOptions.MediaSize;
                ResizeImageForPrint(pageDescription);
                printDoc.InvalidatePreview();
            }

            if (_printOrientation != e.PrintTaskOptions.Orientation)
            {
                _printOrientation = e.PrintTaskOptions.Orientation;
                ResizeImageForPrint(pageDescription);
                printDoc.InvalidatePreview();
            }
            // As I only want to print one Rectangle, so I set the count to 1
            printDoc.SetPreviewPageCount(1, PreviewPageCountType.Final);
        }


        private void ResizeImageForPrint(PrintPageDescription pageDescription)
        {
            if (bitmap == null) return;
            if ((bitmap.Width - pageDescription.ImageableRect.Width) >= (bitmap.Height - pageDescription.ImageableRect.Height))
            {
                Picture.Width = pageDescription.ImageableRect.Width;
            }
            else
            {
                Picture.Height = pageDescription.ImageableRect.Height;
            }
            Picture.HorizontalAlignment = HorizontalAlignment.Left;
            Picture.VerticalAlignment = VerticalAlignment.Top;
        }
        // Print preview

        private void GetPreviewPage(object sender, GetPreviewPageEventArgs e)
        {
            // Provide a UIElement as the print preview.
            try
            {
                printDoc.SetPreviewPage(e.PageNumber, this.Picture);
            }
            catch (System.ArgumentException ex) { }
        }

        //Add pages to send to the printer

        private void AddPages(object sender, AddPagesEventArgs e)
        {
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
            if (args.Completion == PrintTaskCompletion.Submitted || args.Completion == PrintTaskCompletion.Canceled)
            {
                await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    AjustPictureSizeToPictureArea();
                });
            }
        }


        #endregion



        private void Palette_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            RedrawImageArea();
        }

        private void ApplicationStatus_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            // Disable Display Converted/Original toggle switch if image hided 
            ApplicationStatus.ImageConverted = !ApplicationStatus.HideImage;

            if (e.PropertyName == "HideImage" || e.PropertyName == "DisplayConverted" || e.PropertyName == "DisplayContour")
            {
                RedrawImageArea();
            }
        }

        private void RedrawImageArea()
        {
            bitmap.ClearPixelBuffer();
            if (!ApplicationStatus.HideImage)
            {
                if (ApplicationStatus.DisplayConverted)
                {
                    foreach (var color in Palette.ActiveColors)
                    {
                        if (color.Value)
                            bitmap.SetLevelDataToDisplayBuffer(color.Key);
                    }
                }
                else
                    bitmap.SetOriginalImageDataToDisplayBuffer();
            }
            if (ApplicationStatus.DisplayContour)
                bitmap.DisplayContours(Options.ContourColorValue, Options.MinContourLength, Options.ContourDensityValue);

             bitmap.Invalidate();
        }

        private async Task<MsgBoxButton> DisplayMessage(UserMessage message)
        {
            MsgBox.SetButtons(message.Buttons);
            MsgBox.Message = message.Text;
            MsgBox.InnerBoxHeight = message.BoxHeight;
            MsgBox.InnerBoxWidth = message.BoxWidth;
            return await MsgBox.Show();
        }

        private async void FunctionNotImplementerMessage()
        {
            UserMessage msg = new UserMessage(MsgBoxType.Info, "Function not imlemented");
            MsgBoxButton mbb = await DisplayMessage(msg);
        }

        /// <summary>
        /// Load JPG or BMP image to bitmap object
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private async void MenuFileOpen_Clicked(object sender, RoutedEventArgs e)
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
                ApplicationStatus.Reset();

                ApplicationStatus.ImageFileName = file.Name;
                _imageFilePath = file.Path.Replace(ApplicationStatus.ImageFileName, "");
                _imageFileNameExtention = file.FileType;

                ImageProperties props = await file.Properties.GetImagePropertiesAsync();

                bitmap = new ContourBitmap((int)props.Width, (int)props.Height);

                IRandomAccessStream stream = await file.OpenAsync(FileAccessMode.Read);
                bitmap.SetSource(stream);
                Picture.Source = bitmap.ImageData;
                AjustPictureSizeToPictureArea();
                _pictureWidthToHeightRatio = props.Width / props.Height;
                ApplicationStatus.ImageLoaded = true;
                Palette.Clear();
            }
            else
            {
                //ImageFileName = "No loaded files.";
            }
        }

        private async void MenuFileSave_Clicked(object sender, RoutedEventArgs e)
        {
            if (bitmap == null) return;
            StorageFolder folder = await StorageFolder.GetFolderFromPathAsync(_imageFilePath);
            StorageFile file = await folder.CreateFileAsync(ApplicationStatus.ImageFileName, CreationCollisionOption.ReplaceExisting);

            IRandomAccessStream stream = await file.OpenAsync(FileAccessMode.ReadWrite);
            Guid EncoderID = GetEncoderIDFromFileType(_imageFileNameExtention);
            BitmapEncoder encoder = await BitmapEncoder.CreateAsync(EncoderID, stream);
            // Get pixels of the WriteableBitmap object 
            Stream pixelStream = bitmap.ImageData.PixelBuffer.AsStream();
            byte[] pixels = new byte[pixelStream.Length];
            await pixelStream.ReadAsync(pixels, 0, pixels.Length);
            encoder.SetPixelData(BitmapPixelFormat.Bgra8, BitmapAlphaMode.Ignore, bitmap.Width, bitmap.Height, 96.0, 96.0, pixels);
            await encoder.FlushAsync();
        }

        private Guid GetEncoderIDFromFileType(string filetype)
        {
            switch (filetype)
            {
                case ".bmp": return BitmapEncoder.BmpEncoderId;
                case ".jpg": return BitmapEncoder.JpegEncoderId;

            }
            return BitmapEncoder.JpegEncoderId;

        }

        private async void MenuFileSaveAs_Clicked(object sender, RoutedEventArgs e)
        {
            if (bitmap == null) return;
            FileSavePicker picker = new FileSavePicker();
            picker.FileTypeChoices.Add("bmp File", new List<string>() { ".bmp" });
            picker.FileTypeChoices.Add("jpg File", new List<string>() { ".jpg" });
            StorageFile file = await picker.PickSaveFileAsync();
            if (file == null)
                return;
            IRandomAccessStream stream = await file.OpenAsync(FileAccessMode.ReadWrite);

            Guid EncoderID = GetEncoderIDFromFileType(file.FileType);
            BitmapEncoder encoder = await BitmapEncoder.CreateAsync(EncoderID, stream);
            // Get pixels of the WriteableBitmap object 
            Stream pixelStream = bitmap.ImageData.PixelBuffer.AsStream();
            byte[] pixels = new byte[pixelStream.Length];
            await pixelStream.ReadAsync(pixels, 0, pixels.Length);
            encoder.SetPixelData(BitmapPixelFormat.Bgra8, BitmapAlphaMode.Ignore, bitmap.Width, bitmap.Height, 96.0, 96.0, pixels);

            await encoder.FlushAsync();
        }
        #region Showing print dialog
        private async void MenuFilePrint_Clicked(object sender, RoutedEventArgs e)
        {
           
            if (PrintManager.IsSupported())
            {
                try
                {
                    // Show print UI
                    bool v = await PrintManager.ShowPrintUIAsync();
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


        #endregion
        private void MenuFileExit_Clicked(object sender, RoutedEventArgs e)
        {
            Application.Current.Exit();
        }

        private async void MenuFileOptions_Clicked(object sender, RoutedEventArgs e)
        {
            Options.Restore();
            await OptionsWindow.Show();
            ApplicationStatus.NumberOfLevels = Options.NumberOfColors;
            ApplicationStatus.ConversionMode = Options.ConversionTypeName + ".";
        }

        private async void MenuOperationConvert_Clicked(object sender, RoutedEventArgs e)
        {
            if (ApplicationStatus.ImageLoaded)
            {
                Progress.Show(ProgressPanel.ProgressPanelType.ProgressBar);
                IAsyncActionWithProgress<double> asyncAction = null;
                if (Options.ConversionType == TypeOfConvertion.Grayscale)
                {
                    asyncAction = bitmap.ConvertToGrayscaleAsync(Options.NumberOfColors);
                }
                if (Options.ConversionType == TypeOfConvertion.ReducedColors)
                {
                    asyncAction = bitmap.ConvertToReducedColorsAsync(Options.NumberOfColors);
                }

                asyncAction.Progress = new AsyncActionProgressHandler<double>((action, progress) =>
                {
                   Progress.ProgressValue = ((float)progress);
                   
                });
                await asyncAction;

                bitmap.ExtractLevels();
                this.Palette.Build(bitmap.Colors);

                ApplicationStatus.ImageConverted = true;
                ApplicationStatus.DisplayConverted = true;
                bitmap.Invalidate();
                Progress.Hide();
                
            }
            else
            {
                UserMessage message = new UserMessage()
                {
                    Type = MsgBoxType.Error,
                    Text = "Image not loaded.",
                    BoxWidth = 350,
                    BoxHeight = 150
                };
                await DisplayMessage(message);
            }
        }

        private async void MenuOperationClean_Clicked(object sender, RoutedEventArgs e)
        {
            var starttime = DateTime.Now;
            Progress.Show(ProgressPanel.ProgressPanelType.ProgressBar);
            IAsyncActionWithProgress<double> asyncAction = null;
            asyncAction = bitmap.CleanUpImageAsync(Options.CleanupValue);
            asyncAction.Progress = new AsyncActionProgressHandler<double>((action, progress) =>
            {
                Progress.ProgressValue = ((float)progress);
            });
            await asyncAction;

            bitmap.ExtractLevels();
            Palette.Build(bitmap.Colors);
            bitmap.Invalidate();
            Progress.Hide();
            Options.TimeSpended = (DateTime.Now - starttime).TotalMilliseconds;
         }

        private async void MenuOperationOutline_Clicked(object sender, RoutedEventArgs e)
        {
            ApplicationStatus.DisplayContour = false;

            if (ApplicationStatus.ImageConverted)
            {
                ApplicationStatus.ProgressValue = 0;
                int totalProgress = 0;
                ApplicationStatus.ProgressBarVisibility = Visibility.Visible;

                Progress.Show(ProgressPanel.ProgressPanelType.Counter);

                int numberOfLevels = bitmap.LevelsCount;

                ApplicationStatus.NumberOfLevels = numberOfLevels;

                List<IAsyncActionWithProgress<int>> taskList = new List<IAsyncActionWithProgress<int>>();

                var starttime = DateTime.Now;
                foreach (var color in Palette.ActiveColors)
                {
                    IAsyncActionWithProgress<int> asyncAction = null;
                    asyncAction = bitmap.FindLevelContoursAsync(color.Key);
                    asyncAction.Progress = new AsyncActionProgressHandler<int>((action, progress) =>
                    {
                        double i = progress;
                        totalProgress += progress;
                        Progress.CounterValue = totalProgress;
                    });
                    taskList.Add(asyncAction);
                }

                foreach (var task in taskList)
                     await task;
 
                Options.TimeSpended = (DateTime.Now - starttime).TotalMilliseconds;
                Progress.Hide();
                ApplicationStatus.ImageOutlined = true;
                ApplicationStatus.DisplayContour = true;
            }
        }

        private void MenuOperationRotateLeft_Clicked(object sender, RoutedEventArgs e)
        {
            bitmap.RotateLeft();
            Picture.Source = bitmap.ImageData;
           
        }

        private void MenuOperationRotateRight_Clicked(object sender, RoutedEventArgs e)
        {
            bitmap.RotateRight();
            Picture.Source = bitmap.ImageData;
            
        }
        private async void MenuHelpAbout_Clicked(object sender, RoutedEventArgs e)
        {
            await AboutWindow.Show();
        }

    } // End of MainPage class definition

}
