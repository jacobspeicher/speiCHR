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

        public MainWindow()
        {
            InitializeComponent();
            chrFile = new CHRFile();
            fp = new FileProcessor(chrFile);
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
                DrawImage();
            }
        }

        void DrawImage()
        {
            List<Color> colors = new List<Color>();
            colors.Add(Colors.Black);
            colors.Add(Colors.Red);
            colors.Add(Colors.Green);
            colors.Add(Colors.Blue);
            BitmapPalette spritePalette = new BitmapPalette(colors);

            WriteableBitmap sprites = new WriteableBitmap(8, 8, 96, 96, PixelFormats.Indexed2, spritePalette);

            uint[] pixels = new uint[8 * 2];

            for (int i = 0; i < 8; ++i)
            {
                for (int j = 0; j < 2; ++j)
                {
                    pixels[8 * j + i] = 1;
                }
            }

            sprites.WritePixels(new Int32Rect(0, 0, 8, 8), pixels, 2, 16);
            SpriteImage.Source = sprites;
        }

        void OpenCmdCanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }
    }
}
