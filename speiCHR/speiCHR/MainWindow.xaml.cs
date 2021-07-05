using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using speiCHR.Files;

namespace speiCHR
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private FileProcessor fp;
        private CHRFile chrFile;
        private Palette[] palettes = new Palette[4];
        private Brush curBrush;

        public MainWindow()
        {
            InitializeComponent();
            speiChrSetup();
            spriteGridSetup();
            paletteGridSetup();
            
            CommandBinding OpenCmdBinding = new CommandBinding(
                ApplicationCommands.Open,
                OpenCmdExecuted,
                OpenCmdCanExecute
            );

            Binding FileNameBinding = new Binding("Name")
            {
                Source = chrFile,
                Mode = BindingMode.OneWay
            };
            tbl_FileName.SetBinding(TextBlock.TextProperty, FileNameBinding);

            this.CommandBindings.Add(OpenCmdBinding);
        }

        private void speiChrSetup()
        {
            chrFile = new CHRFile();
            fp = new FileProcessor(chrFile);
            palettes[0] = new Palette();
            curBrush = palettes[0].Colors[0];
        }

        private void spriteGridSetup()
        {
            SpriteGrid.Height = 1024;
            SpriteGrid.Width = 1024;

            Brush fill = palettes[0].Colors[0];
            Brush stroke = palettes[0].Colors[4];

            for (int i = 0; i < 128; ++i)
            {
                ColumnDefinition colDef = new ColumnDefinition();
                RowDefinition rowDef = new RowDefinition();

                colDef.Width = new GridLength(8);
                rowDef.Height = new GridLength(8);

                SpriteGrid.ColumnDefinitions.Add(colDef);
                SpriteGrid.RowDefinitions.Add(rowDef);
            }

            for (int i = 0; i < 64; ++i)
            {
                for (int j = 0; j < 64; ++j)
                {
                    Rectangle px = new Rectangle();
                    px.Fill = fill;
                    px.Stroke = stroke;
                    px.Height = 8;
                    px.Width = 8;
                    px.MouseLeftButtonDown += new MouseButtonEventHandler(FillPixel);
                    Grid.SetRow(px, i);
                    Grid.SetColumn(px, j);

                    SpriteGrid.Children.Add(px);
                }
            }
        }

        private void paletteGridSetup()
        {
            PaletteGrid.Height = 25;
            PaletteGrid.Width = 100;

            for(int i = 0; i < 4; ++i)
            {
                ColumnDefinition colDef = new ColumnDefinition();
                colDef.Width = new GridLength(25);
                PaletteGrid.ColumnDefinitions.Add(colDef);
            }
            RowDefinition rowDef = new RowDefinition();
            rowDef.Height = new GridLength(25);
            PaletteGrid.RowDefinitions.Add(rowDef);

            for(int i = 0; i < 4; ++i)
            {
                Rectangle rect = new Rectangle();
                rect.Fill = palettes[0].Colors[i];
                rect.Height = 25;
                rect.Width = 25;
                rect.MouseLeftButtonDown += new MouseButtonEventHandler(SelectPaletteColor);
                Grid.SetRow(rect, 0);
                Grid.SetColumn(rect, i);

                PaletteGrid.Children.Add(rect);
            }
        }

        void OpenCmdExecuted(object target, ExecutedRoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
            dlg.DefaultExt = ".chr";
            dlg.Filter = "CHR Files (.chr)|*.chr";

            // Show the open file dialog box
            Nullable<bool> result = dlg.ShowDialog();

            // Process open file dialog box results
            if (result == true)
            {
                // Open document
                string filename = dlg.FileName;
                string output = fp.ReadFiles(filename);
                MessageBox.Show(output);
                DrawTables();
            }
        }

        private void DrawTables()
        {
            for(int i = 0; i < 8; ++i)
            {
                for(int j = 0; j < 8; ++j)
                {
                    int pxPos = i * 8 + j;
                    int paletteColor = chrFile.Pixels[i, 7 - j];
                    Rectangle px = (Rectangle)SpriteGrid.Children[pxPos];
                    px.Fill = palettes[0].Colors[paletteColor];
                }
            }
        }

        void OpenCmdCanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        void FillPixel(object sender, RoutedEventArgs e)
        {
            Rectangle rect = (Rectangle)sender;

            rect.Fill = curBrush;
        }

        void SelectPaletteColor(object sender, RoutedEventArgs e)
        {
            Rectangle rect = (Rectangle)sender;
            curBrush = rect.Fill;
        }
    }
}
