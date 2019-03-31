using System;
using Windows.Storage;
using Windows.Storage.FileProperties;
using Windows.Storage.Pickers;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media.Imaging;

using System.Runtime.InteropServices.WindowsRuntime;

using ContourHelpers;
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
        //private WriteableBitmap wbitmap = null;
        private ContourHelpers.Bitmap bitmap = null;

        public MainPage()
        {
            this.InitializeComponent();

            for (byte i=2; i<17; i++)
                cbx_levels.Items.Add(i.ToString());

            cbx_levels.SelectedIndex =0;

            
        }

        private async void BtnLoadImage_TappedAsync(object sender, TappedRoutedEventArgs e)
        {
            FileOpenPicker openPicker = new FileOpenPicker();
            openPicker.SuggestedStartLocation = PickerLocationId.PicturesLibrary;
            openPicker.FileTypeFilter.Add(".jpg");
            openPicker.FileTypeFilter.Add(".bmp");

            StorageFile file = await openPicker.PickSingleFileAsync();

            if (file != null)
            {
                ImageProperties props = await file.Properties.GetImagePropertiesAsync();

                bitmap = new Bitmap(this, (int)props.Width, (int)props.Height);

                IRandomAccessStream stream = await  file.OpenAsync(FileAccessMode.Read);
                bitmap.SetSource(stream);
                OutlineImage.Source = bitmap.ImageData;

                bitmap.ConvertToGrayscale(byte.Parse((cbx_levels.SelectedValue.ToString())));
                bitmap.ExtractLevels();
                BuildLayersWindow();
                tbl_Result.Text = "Image loaded";
            }
            else
            {
                tbl_Result.Text = "Image not loaded";
            }
        }

        private void BtnConvertToGrayScale_Tapped(object sender, TappedRoutedEventArgs e)
        {
            byte levels = byte.Parse((cbx_levels.SelectedValue.ToString()));

            bitmap.ConvertToGrayscale(levels);
            bitmap.ImageData.Invalidate();
            tbl_Result.Text = "Conversion copleted";
        }

        private void BtnExtractLevels_Tapped(object sender, TappedRoutedEventArgs e)
        {


            BuildLayersWindow();

            //    bitmap.ExtractLevels();

            //    foreach (Level level in bitmap.Levels)
            //        cbx_color.Items.Add(level.Color.ToString());
            //    tbl_Result.Text = "Levels extracted";
            //
        }

        private void CbxCcolor_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
//            string level = cbx_color.SelectedValue.ToString();
//            bitmap.DisplayLevel(byte.Parse(level));
//            bitmap.ImageData.Invalidate();
//            tbl_Result.Text = string.Format("Level {0} selected", level) ;
        }

        private void BtnOutlineImage_Tapped(object sender, TappedRoutedEventArgs e)
        {
            bitmap.OutlineImage();
            tbl_Result.Text = string.Format("Image outlined");
        }

        private void BtnClearImage_Tapped(object sender, TappedRoutedEventArgs e)
        {
 //           string color = cbx_color.SelectedValue.ToString();
 

 //           bitmap.RectifyLevel(byte.Parse(color), int.Parse(size));

 //           tbl_Result.Text = string.Format("Level {0} cleared", color);

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
                ldp.OutlineButtonTapped += OnOutlineButtonTapped;
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

        private void OnOutlineButtonTapped(object obj, TappedRoutedEventArgs e)
        {
            ;
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
