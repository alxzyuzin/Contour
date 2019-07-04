using System;
using Windows.Storage;
using Windows.Storage.FileProperties;
using Windows.Storage.Pickers;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;
using System.Runtime.InteropServices.WindowsRuntime;

using ContourExtractorWindowsRuntimeComponent;
using Windows.Storage.Streams;
using System.Collections.Generic;
using Windows.Graphics.Imaging;
using System.IO;
using Windows.UI.Xaml;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace Contour
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private ContourBitmap bitmap = null;
        public byte LevelsNumber { get; set; } = 2;
        public string ImageFileName { get; set; } = string.Empty;

        public MainPage()
        {
            this.InitializeComponent();
        }

        private async void BtnLoadImage_TappedAsync(object sender, TappedRoutedEventArgs e)
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
                ImageFileName = file.DisplayName;
                ImageProperties props = await file.Properties.GetImagePropertiesAsync();

                bitmap = new ContourBitmap(this, (int)props.Width, (int)props.Height);

                IRandomAccessStream stream = await  file.OpenAsync(FileAccessMode.Read);
                bitmap.SetSource(stream);
                OutlineImage.Source = bitmap.ImageData;
                string a = ((ComboBoxItem)cbx_levels.SelectedValue).Content.ToString();
                bitmap.ConvertToGrayscale(byte.Parse(a));
                bitmap.ExtractLevels();
                BuildLayersWindow();
                tbl_Result.Text = $"Image loaded {ImageFileName}"; 
            }
            else
            {
                tbl_Result.Text = "Operation canceled";
            }
        }

        private void BtnExtractLevels_Tapped(object sender, TappedRoutedEventArgs e)
        {
            BuildLayersWindow();
        }


         private async void BtnSaveImage_Tapped(object sender, TappedRoutedEventArgs e)
        {
             FileSavePicker picker = new FileSavePicker();
             picker.FileTypeChoices.Add("bmp File", new List<string>() { ".bmp" });
            StorageFile savefile = await picker.PickSaveFileAsync();
            if (savefile == null)
                return;
            IRandomAccessStream stream = await savefile.OpenAsync(FileAccessMode.ReadWrite);
            BitmapEncoder encoder = await BitmapEncoder.CreateAsync(BitmapEncoder.BmpEncoderId, stream);
            // Get pixels of the WriteableBitmap object 

            Stream pixelStream = bitmap.ImageData.PixelBuffer.AsStream();
            byte[] pixels = new byte[pixelStream.Length];
            await pixelStream.ReadAsync(pixels, 0, pixels.Length);
            // Save the image file with jpg extension 
            encoder.SetPixelData(BitmapPixelFormat.Bgra8, BitmapAlphaMode.Ignore, (uint)bitmap.ImageData.PixelWidth, (uint)bitmap.ImageData.PixelHeight, 96.0, 96.0, pixels);
            await encoder.FlushAsync();
        }

        private void BuildLayersWindow()
        {
            Button b = new Button();

            int lcount = stp_Layers.Children.Count;
            for (int i = 0; i < lcount; i++)
                stp_Layers.Children.RemoveAt(0);

            foreach (byte color in bitmap.GrayScaleColorMap)
            {
                LayerDisplayParams ldp =  new LayerDisplayParams(color);
                ldp.ContourSwitchToggled += OnContourSwitchToggled;
                ldp.ShapeSwitchToggled += OnShapeSwitchToggled;
                stp_Layers.Children.Add(ldp);
            }
        }

        private DisplayParams[] BuilDisplayParamsArray()
        {
             DisplayParams[] parameters = new DisplayParams[stp_Layers.Children.Count];

            for (int i = 0; i < stp_Layers.Children.Count; i++)
            {
                DisplayParams displayParams = new DisplayParams();
                
                LayerDisplayParams userParams = (LayerDisplayParams)  stp_Layers.Children[i];

                displayParams.Color = userParams.Color;
                displayParams.DisplayShapes = userParams.DisplayShapes;
                displayParams.DisplayContours = userParams.DisplayContours;

                parameters.SetValue(displayParams, i);
            }
            return parameters;
        }

        private void BtnOutlineImage_Tapped(object sender, TappedRoutedEventArgs e)
        {
            if (bitmap == null)
                return;
             bitmap.ConvertToGrayscale(LevelsNumber);
            bitmap.ExtractLevels();
            bitmap.OutlineImage();

            tbl_Result.Text = "Image {ImageFileName} outlined.";
        }

        private void OnShapeSwitchToggled(object obj, RoutedEventArgs e)
        {
            DisplayParams[] parameters = BuilDisplayParamsArray();
            bitmap.DisplayOutlinedImage(parameters);
            bitmap.ImageData.Invalidate();
        }

        private void OnContourSwitchToggled(object obj, RoutedEventArgs e)
        {
            DisplayParams[] parameters = BuilDisplayParamsArray();
            bitmap.DisplayOutlinedImage(parameters);
            bitmap.ImageData.Invalidate();
        }
        
        private void BtnRestoreOriginalImage_Tapped(object sender, TappedRoutedEventArgs e)
        {
            bitmap.RestoreOriginalImage();
            bitmap.ImageData.Invalidate();
        }
    }

    
}
