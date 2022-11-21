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
        private Image<Gray, byte> graySource;
        private Image<Bgr, byte> source;
        private double Blue_threshold;
        private double Green_threshold;
        private double Red_threshold;

        public MainWindow()
        {
            InitializeComponent();
            var fileName = "C:/2.jpg";
            sourceImage.Source = new BitmapImage(new Uri(fileName));
            source = new Image<Bgr, byte>(fileName);
            graySource = new Image<Gray, byte>(fileName);
        }

        private void Slider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (source == null)
            {
                return;
            }

            var result = graySource.ThresholdBinary(new Gray(e.NewValue), new Gray(255));
            resultImage.Source = result.ToBitmapSource();
        }

        private void Slider_ValueChanged_1(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (source == null)
            {
                return;
            }

            Red_threshold = e.NewValue;
            RgbThreashold();
        }

        private void Slider_ValueChanged_2(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (source == null)
            {
                return;
            }

            Green_threshold = e.NewValue;
            RgbThreashold();
        }

        private void Slider_ValueChanged_3(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (source == null)
            {
                return;
            }

            Blue_threshold = e.NewValue;
            RgbThreashold();
        }

        private void RgbThreashold()
        {
            using var result = source.ThresholdBinary(new Bgr(Blue_threshold, Green_threshold, Red_threshold),
                new Bgr(255, 255, 255));

            resultImage.Source = result.ToBitmapSource();
        }
    }
}
