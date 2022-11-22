using Emgu.CV;
using Emgu.CV.CvEnum;
using Emgu.CV.Features2D;
using Emgu.CV.Linemod;
using Emgu.CV.Structure;
using Emgu.CV.Util;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Runtime.CompilerServices;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media.Imaging;

namespace CvTest.ViewModels
{
    public class MainViewModel : INotifyPropertyChanged
    {
        private double _grayThreshold;
        private double _redThreshold;
        private double _greenThreshold;
        private double _blueThreshold;
        private float _minArea;
        private float _maxArea;
        private float _minDist;
        private bool _filterByColor;
        private byte _color;
        private ICommand _contourDetectionCommand;
        private ICommand _simpleBlobDetectorCommand;

        public string SourceImagePath { get; set; }
        public BitmapSource ResultImage { get; set; }

        public Image<Gray, byte> GrayImage { get; set; }
        public Image<Bgr, byte> SourceImage { get; set; }
        public double GrayThreshold
        {
            get => _grayThreshold;
            set
            {
                _grayThreshold = value;
                BinaryThreshold();
            }
        }
        public double BlueThreshold
        {
            get => _blueThreshold;
            set
            {
                _blueThreshold = value;
                RgbThreshold();
            }
        }
        public double GreenThreshold
        {
            get => _greenThreshold;
            set
            {
                _greenThreshold = value;
                RgbThreshold();
            }
        }
        public double RedThreshold
        {
            get => _redThreshold; set
            {
                _redThreshold = value;
                RgbThreshold();
            }
        }

        public float MinArea
        {
            get => _minArea;
            set
            {
                _minArea = value;
                Detector();
            }
        }

        public float MaxArea
        {
            get => _maxArea;
            set
            {
                _maxArea = value;
                Detector();
            }
        }

        public float MinDist
        {
            get => _minDist;
            set
            {
                _minDist = value;
                Detector();
            }
        }

        public bool FilterByColor
        {
            get => _filterByColor;
            set
            {
                _filterByColor = value;
                Detector();
            }
        }

        public byte Color
        {
            get => _color;
            set
            {
                _color = value;
                Detector();
            }
        }

        public long FoundContoursCount { get; set; }

        public ICommand ContourDetectionCommand => _contourDetectionCommand ??= new Command(ContourDetection);
        public ICommand SimpleBlobDetectorCommand => _simpleBlobDetectorCommand ??= new Command(SimpleBlobDetector);

        public MainViewModel()
        {
            LoadImage();

            MinDist = 22;
            MinArea = 35;
            MaxArea = 988;
        }

        public void LoadImage()
        {
            SourceImagePath = "D:/2.jpg";
            SourceImage = new Image<Bgr, byte>(SourceImagePath);
            GrayImage = new Image<Gray, byte>(SourceImagePath);
        }

        private Image<Gray, byte> BinaryThreshold()
        {
            var result = GrayImage.ThresholdBinary(new Gray(GrayThreshold), new Gray(255));

            ResultImage = result.ToBitmapSource();

            return result;
        }

        private Image<Bgr, byte> RgbThreshold()
        {
            var result = SourceImage.ThresholdBinary(new Bgr(BlueThreshold, GreenThreshold, RedThreshold),
               new Bgr(255, 255, 255));
            ResultImage = result.ToBitmapSource();

            return result;
        }

        private void Detector()
        {
            var detectorParams = new SimpleBlobDetectorParams()
            {
                FilterByInertia = false,

                FilterByConvexity = true,
                MinConvexity = 0.5f,
                MaxConvexity = 1.1f,

                FilterByColor = FilterByColor,
                blobColor = Color,

                FilterByArea = true,
                MinArea = MinArea,
                MaxArea = MaxArea,
                MinDistBetweenBlobs = MinDist,

                MinThreshold = 150,
                MaxThreshold = 254,

                FilterByCircularity = true,
                MinCircularity = 0.5f,
                MaxCircularity = 1.1f,
            };
            var detector = new SimpleBlobDetector(detectorParams);

            var result = detector.Detect(SourceImage);
            var outImg = SourceImage.Clone();

            var black = new MCvScalar(0, 0, 0);
            var white = new MCvScalar(255, 255, 255);
            var textColor = new MCvScalar(0, 255, 255);
            foreach (var point in result)
            {
                //TODO: Round correctly
                var x = (int)point.Point.X;
                var y = (int)point.Point.Y;
                var size = (int)point.Size;
                var radius = size / 2;

                SourceImage.ROI = new Rectangle(x - radius, y - radius, size, size);
                var cuttedImage = SourceImage.Copy();
                var mask = new Image<Gray, byte>(size, size);
                CvInvoke.Circle(mask, new Point(radius, radius), radius, white, -1, LineType.AntiAlias, 0);

                var mean = CvInvoke.Mean(cuttedImage, mask);
                var b = mean.V0;
                var g = mean.V1;
                var r = mean.V2;
                var text = r > g && r > b ? "R" : b > g && b > r ? "B" : g > r && g > b ? "G" : "N/A";

                CvInvoke.Circle(outImg, new Point(x, y), radius, black, 2);
                CvInvoke.PutText(outImg, text, new Point(x, y + 2 * size), FontFace.HersheySimplex, 1.0, black, 8);
                CvInvoke.PutText(outImg, text, new Point(x, y + 2 * size), FontFace.HersheySimplex, 1.0, textColor, 2);
            }

            FoundContoursCount = result?.Length ?? 0;

            //Old method to draw contours
            //Mat outimg = new Mat();
            //Features2DToolbox.DrawKeypoints(SourceImage, new VectorOfKeyPoint(result), outimg, new Bgr(0, 0, 0), Features2DToolbox.KeypointDrawType.DrawRichKeypoints);

            ResultImage = outImg.ToBitmapSource();
        }

        private void ContourDetection(object param)
        {
            var result = BinaryThreshold();
            VectorOfVectorOfPoint contours = new VectorOfVectorOfPoint();

            Mat hierarchy = new Mat();

            CvInvoke.FindContours(result, contours, hierarchy, Emgu.CV.CvEnum.RetrType.Tree, Emgu.CV.CvEnum.ChainApproxMethod.ChainApproxSimple);
            FoundContoursCount = contours.Length;
            CvInvoke.DrawContours(SourceImage, contours, -1, new MCvScalar(0, 0, 255), 5);
            ResultImage = SourceImage.ToBitmapSource();
        }

        private void SimpleBlobDetector(object para)
        {
            Size patternSize = new Size(40, 30);
            var detectorParams = new SimpleBlobDetectorParams()
            {
                FilterByCircularity = true,
                FilterByArea = true,
                MinCircularity = 0.7f,
                MaxCircularity = 1.0f,
                MinArea = 500,
                MaxArea = 10000,
            };
            var simpleBlobDetector = new SimpleBlobDetector(detectorParams);
            var centers = new VectorOfPointF();

            var found = CvInvoke.FindCirclesGrid(BinaryThreshold(), patternSize, Emgu.CV.CvEnum.CalibCgType.SymmetricGrid | Emgu.CV.CvEnum.CalibCgType.Clustering, simpleBlobDetector);
            CvInvoke.DrawChessboardCorners(SourceImage, patternSize, centers, found != null);

            ResultImage = SourceImage.ToBitmapSource();
        }


        public event PropertyChangedEventHandler PropertyChanged;
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
            => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}
