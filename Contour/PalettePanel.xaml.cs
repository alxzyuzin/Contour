/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 *
 * This file is part of Contour project.
 *
 * This class presents set of colors in image converted to grayscale image or
 * image with redused number of colors
 *
 ---------------------------------------------------------------------------------*/

using ContourExtractorWindowsRuntimeComponent;
using ContourUI;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media;


// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace ContourUI
{

    public sealed class IntColor
    {
        uint _color;
        public IntColor(uint color)
        {
            _color = color;
        }

        public byte Alfa
        {
            get
            {
                byte r;
                uint c = _color;
                c = c & 0xFF000000;
                c = c >> 24;
                r = (byte)c;
                return r;
            }
            set
            {
                uint c = value;
                c = c << 24;
                _color = _color & 0x00FFFFFF;
                _color = _color | c;
            }
        }
        public byte Red
        {
            get
            {
                byte r;
                uint c = _color;
                c = c & 0x00FF0000;
                c = c >> 16;
                r = (byte)c;
                return r;
            }
            set
            {

                byte r = value;
                uint c = value;
                c = c << 16;
                _color = _color & 0xFF00FFFF;
                _color = _color | c;
            }
        }
        public byte Green
        {
            get
            {
                byte r;
                uint c = _color;
                c = c & 0x0000FF00;
                c = c >> 8;
                r = (byte)c;
                return r;
            }
            set
            {
                uint c = value;
                c = c << 8;
                _color = _color & 0xFFFF00FF;
                _color = _color | c;
            }
        }
        public byte Blue
        {
            get
            {
                byte r;
                uint c = _color;
                c = c & 0x000000FF;

                r = (byte)c;
                return r;
            }
            set
            {
                uint c = value;
                _color = _color & 0xFFFFFF00;
                _color = _color | c;
            }
        }
    }
    public sealed partial class PalettePanel : UserControl, INotifyPropertyChanged
    {

        private SortedDictionary<uint, bool> _activeColors = new SortedDictionary<uint, bool>();

        public event PropertyChangedEventHandler PropertyChanged;

        public PalettePanel() 
        {
            this.InitializeComponent();
        }
       
        public SortedDictionary<uint,bool> ActiveColors
        {
            get =>  _activeColors;
        }

        public void Build(uint[] colors)
        {
            this.Clear();
            List<uint> colorsList = new System.Collections.Generic.List<uint>(colors);
            colorsList.Sort();
            // Define number of palette columns and rows
            int paletteRows = GetNumberOfPaletteRows(colors.Length);
            int paletteColumns = GetNumberOfPaletteColumns(colors.Length);

            for (int r = 0; r < paletteRows; r++)
            {
                RowDefinition row = new RowDefinition();
                row.MaxHeight = 40;
                GridPalette.RowDefinitions.Add(row);
            }
            for (int c = 0; c < paletteColumns; c++)
            {
                ColumnDefinition col = new ColumnDefinition();
                col.MaxWidth = 80;
                GridPalette.ColumnDefinitions.Add(col);
            }
         
            int i = 0;
            for (int r = 0; r < paletteRows; r++)
                for (int c = 0; c < paletteColumns; c++)
                {
                    if (i < colors.Length)
                    {
                        Windows.UI.Xaml.Shapes.Rectangle paletteItem = BuildPaletteItem(colors[i++]);
                        GridPalette.Children.Add(paletteItem);
                        Grid.SetColumn(paletteItem, c);
                        Grid.SetRow(paletteItem, r);
                    }
                }
        }

        public void Build2(uint[] colors)
        {
            this.Clear();
            List<uint> colorsList = new System.Collections.Generic.List<uint>(colors);
            colorsList.Sort();
            // Define number of palette columns and rows
            int paletteRows = GetNumberOfPaletteRows(colors.Length);
            int paletteColumns = GetNumberOfPaletteColumns(colors.Length);

            for (int r = 0; r < paletteRows; r++)
            {
                RowDefinition row = new RowDefinition();
                row.MaxHeight = 40;
                GridPalette.RowDefinitions.Add(row);
            }
            for (int c = 0; c < paletteColumns; c++)
            {
                ColumnDefinition col = new ColumnDefinition();
                col.MaxWidth = 80;
                GridPalette.ColumnDefinitions.Add(col);
            }

            int i = 0;
            for (int r = 0; r < paletteRows; r++)
                for (int c = 0; c < paletteColumns; c++)
                {
                    if (i < colors.Length)
                    {
                        PaletteItem paletteItem = new PaletteItem(colors[i]);
                        paletteItem.PropertyChanged += PaletteItem_PropertyChanged;
                        GridPalette.Children.Add(paletteItem);
                        Grid.SetColumn(paletteItem, c);
                        Grid.SetRow(paletteItem, r);
                        _activeColors.Add(colors[i], true);
                        i++;
                    }
                }
        }

        private void PaletteItem_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            PaletteItem s = (PaletteItem)sender;
            _activeColors[s.Color] = s.IsChecked;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ActiveColors)));
        }
        /// <summary>
        ///  Remove all palette items from palette, rows and columns
        /// </summary>
        public void Clear()
        {
            GridPalette.Children.Clear();
            GridPalette.RowDefinitions.Clear();
            GridPalette.ColumnDefinitions.Clear();
            _activeColors.Clear();
        }

        private int GetNumberOfPaletteColumns(int NumberOfColors)
        {
            if (NumberOfColors == 128)
                return 8;
            
            if (NumberOfColors <= 4)
                return NumberOfColors;
            
            return 4;
        }

        private int GetNumberOfPaletteRows(int NumberOfColors)
        {
            int NumberOfRows = 0;

            if (NumberOfColors == 128)
                return (int)( NumberOfColors / 8);
            NumberOfRows = NumberOfColors / 4;
            if ((NumberOfColors % 4) != 0)
                ++NumberOfRows;
            
            return NumberOfRows;
        }

        Windows.UI.Xaml.Shapes.Rectangle BuildPaletteItem(uint color)
        {
            Windows.UI.Xaml.Shapes.Rectangle rectangle = new Windows.UI.Xaml.Shapes.Rectangle();
            Thickness margin = rectangle.Margin;
            margin.Left = 1;
            margin.Right = 1;
            margin.Top = 1;
            margin.Bottom = 1;
            rectangle.Margin = margin;

            IntColor currentColor = new IntColor(color);
           
            Windows.UI.Color itemColor = new Windows.UI.Color();
            itemColor.R = currentColor.Red;
            itemColor.G = currentColor.Green;
            itemColor.B = currentColor.Blue;
            itemColor.A = currentColor.Alfa;

            rectangle.Fill = new SolidColorBrush(itemColor);

            return rectangle;
        }

        private void Button_ShowAllTapped(object sender, Windows.UI.Xaml.Input.TappedRoutedEventArgs e)
        {
            foreach (PaletteItem  paletteItem in GridPalette.Children)
            {
                paletteItem.SetIsChecked(true);
                _activeColors[paletteItem.Color] = true;
            }
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ActiveColors)));

        }

        private void Button_HideAllTapped(object sender, Windows.UI.Xaml.Input.TappedRoutedEventArgs e)
        {
            foreach (PaletteItem paletteItem in GridPalette.Children)
            {
                paletteItem.SetIsChecked(false);
                _activeColors[paletteItem.Color] = false;
            }
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ActiveColors)));
        }
    }
       
 }


/* Create greed dinamically
 * 
 Grid grid = new Grid();
grid.Width = 200;
grid.Height = 50;
grid.Margin = new Thickness(0, 50, 0, 0);
ColumnDefinition col1 = new ColumnDefinition();
ColumnDefinition col2 = new ColumnDefinition();
ColumnDefinition col3 = new ColumnDefinition();
col1.Width = new GridLength(0, GridUnitType.Auto);
col2.Width = new GridLength(0, GridUnitType.Auto);
col3.Width = new GridLength(1, GridUnitType.Star);
grid.ColumnDefinitions.Add(col1);
grid.ColumnDefinitions.Add(col2);
grid.ColumnDefinitions.Add(col3);
CheckBox cbox = new CheckBox();
cbox.MinWidth = 32;
cbox.HorizontalAlignment = HorizontalAlignment.Left;
cbox.Background = new SolidColorBrush(Windows.UI.Colors.Transparent);
TextBlock tblock = new TextBlock();
tblock.FontSize = 16;
tblock.HorizontalAlignment = HorizontalAlignment.Left;
tblock.VerticalAlignment = VerticalAlignment.Center;
tblock.Text = "text";
TextBox tbox = new TextBox();
tbox.FontSize = 16;
tbox.HorizontalAlignment = HorizontalAlignment.Left;
tbox.VerticalAlignment = VerticalAlignment.Center;
grid.Children.Add(cbox);
grid.Children.Add(tblock);
grid.Children.Add(tbox);
Grid.SetColumn(cbox, 0);
Grid.SetColumn(tblock, 1);
Grid.SetColumn(tbox, 2);
 */