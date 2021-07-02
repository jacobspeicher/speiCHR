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

        public MainWindow()
        {
            InitializeComponent();
            chrFile = new CHRFile();
            fp = new FileProcessor(chrFile);
            palettes[0] = new Palette();
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
        }

        void OpenCmdCanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        void ButtonClick(object sender, RoutedEventArgs e)
        {
            Button btn = (Button)sender;
            btn.Background = Brushes.Pink;
        }

        void RectClick(object sender, RoutedEventArgs e)
        {
            Rectangle rect = (Rectangle)sender;

            rect.Fill = palettes[0].Colors[1];
        }
    }
}
