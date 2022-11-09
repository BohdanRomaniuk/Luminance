using Emgu.CV.Structure;
using Emgu.CV;
using Microsoft.Win32;
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

namespace CvTest
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        Image<Gray, byte> source;

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            var open = new OpenFileDialog();
            open.Filter = "Image Files (*.tif; *.dcm; *.jpg; *.jpeg; *.bmp)|*.tif; *.dcm; *.jpg; *.jpeg; *.bmp";
            var threshold_value = 50;
            if (open.ShowDialog() == true)
            {
                sourceImage.Source = new BitmapImage(new Uri(open.FileName));
                source = new Image<Gray, byte>(open.FileName);
                var img = source.ThresholdBinary(new Gray(threshold_value), new Gray(255));
                resultImage.Source = img.ToBitmapSource();
            }
        }

        private void Slider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (source != null)
            {
                using (var Gray = source.ThresholdBinary(new Gray(e.NewValue), new Gray(255)))
                {
                    resultImage.Source = Gray.ToBitmapSource();
                }
            }
        }
    }
}
