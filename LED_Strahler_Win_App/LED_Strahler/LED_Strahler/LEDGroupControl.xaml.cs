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
using System.ComponentModel;
using System.Runtime.CompilerServices;
using Xceed.Wpf.Toolkit;
using ColorMine.ColorSpaces;

namespace LED_Strahler_GUI
{
    /// <summary>
    /// Interaktionslogik für LEDGroupControl.xaml
    /// </summary>
    public partial class LEDGroupControl : UserControl, INotifyPropertyChanged
    {
        #region Property changed

        public event PropertyChangedEventHandler PropertyChanged;

        // This method is called by the Set accessor of each property.  
        // The CallerMemberName attribute that is applied to the optional propertyName  
        // parameter causes the property name of the caller to be substituted as an argument.  
        private void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        #endregion

        #region Properties

        /// <summary>
        /// Group name of the control
        /// </summary>
        private string _Group = "Broadcast";
        public string Group
        {
            get { return _Group; }
            set { _Group = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Color from the color picker
        /// </summary>
        public Color ColorPickerColor
        {
            get { return Color.FromRgb(Convert.ToByte(RedValue / 257.0), Convert.ToByte(GreenValue / 257.0), Convert.ToByte(BlueValue / 257.0)); }
            set
            {
                if((value.R != Convert.ToByte(RedValue / 257.0)) || (value.G != Convert.ToByte(GreenValue / 257.0)) || (value.B != Convert.ToByte(BlueValue / 257.0)))
                {
                    RedValue = 257 * value.R;
                    GreenValue = 257 * value.G;
                    BlueValue = 257 * value.B;
                    NotifyPropertyChanged();
                    UpdateHsvFromRgb();
                }
            }
        }

        /// <summary>
        /// Red color value
        /// </summary>
        private ushort _RedValue = 0;
        public ushort RedValue
        {
            get { return _RedValue; }
            set
            {
                if (_RedValue != value)
                {
                    _RedValue = value;
                    NotifyPropertyChanged();
                    UpdateHsvFromRgb();
                }
            }
        }

        /// <summary>
        /// Green color value
        /// </summary>
        private ushort _GreenValue = 0;
        public ushort GreenValue
        {
            get { return _GreenValue; }
            set
            {
                if (_GreenValue != value)
                {
                    _GreenValue = value;
                    NotifyPropertyChanged();
                    UpdateHsvFromRgb();
                }
            }
        }

        /// <summary>
        /// Blue color value
        /// </summary>
        private ushort _BlueValue = 0;
        public ushort BlueValue
        {
            get { return _BlueValue; }
            set
            {
                if(_BlueValue != value)
                {
                    _BlueValue = value;
                    NotifyPropertyChanged();
                    UpdateHsvFromRgb();
                }
            }
        }

        /// <summary>
        /// Hue min slider value
        /// </summary>
        private ushort _HueMinValue = 0;
        public ushort HueMinValue
        {
            get { return _HueMinValue; }
            set
            {
                if(_HueMinValue != value)
                {
                    if (_HueMaxValue < value)
                    {
                        HueMaxValue = value;
                    }
                    _HueMinValue = value;
                    NotifyPropertyChanged();
                }
            }
        }

        /// <summary>
        /// Hue max slider value
        /// </summary>
        private ushort _HueMaxValue = 0;
        public ushort HueMaxValue
        {
            get { return _HueMaxValue; }
            set
            {
                if(_HueMaxValue != value)
                {
                    _HueMaxValue = value;
                    if (_HueMinValue > value)
                    {
                        HueMinValue = value;
                    }
                    NotifyPropertyChanged();
                    UpdateRgbFromHsv();
                }
            }
        }

        /// <summary>
        /// Saturation min slider value
        /// </summary>
        private ushort _SaturationMinValue = 0;
        public ushort SaturationMinValue
        {
            get { return _SaturationMinValue; }
            set
            {
                if(_SaturationMinValue != value)
                {
                    if (_SaturationMaxValue < value)
                    {
                        SaturationMaxValue = value;
                    }
                    _SaturationMinValue = value;
                    NotifyPropertyChanged();
                }
            }
        }

        /// <summary>
        /// Saturation max slider value
        /// </summary>
        private ushort _SaturationMaxValue = 0;
        public ushort SaturationMaxValue
        {
            get { return _SaturationMaxValue; }
            set
            {
                if(_SaturationMaxValue != value)
                {
                    _SaturationMaxValue = value;
                    if (_SaturationMinValue > value)
                    {
                        SaturationMinValue = value;
                    }
                    NotifyPropertyChanged();
                    UpdateRgbFromHsv();
                }
            }
        }

        /// <summary>
        /// Value min slider value
        /// </summary>
        private ushort _ValueMinValue = 0;
        public ushort ValueMinValue
        {
            get { return _ValueMinValue; }
            set
            {
                if(_ValueMinValue != value)
                {
                    if (_ValueMaxValue < value)
                    {
                        ValueMaxValue = value;
                    }
                    _ValueMinValue = value;
                    NotifyPropertyChanged();
                }
            }
        }

        /// <summary>
        /// Value max slider value
        /// </summary>
        private ushort _ValueMaxValue = 0;
        public ushort ValueMaxValue
        {
            get { return _ValueMaxValue; }
            set
            {
                if(_ValueMaxValue != value)
                {
                    _ValueMaxValue = value;
                    if (_ValueMinValue > value)
                    {
                        ValueMinValue = value;
                    }
                    NotifyPropertyChanged();
                    UpdateRgbFromHsv();
                }
            }
        }

        /// <summary>
        /// Period slider value for strobes and fading
        /// </summary>
        private ushort _PeriodValue = 0;
        public ushort PeriodValue
        {
            get { return _PeriodValue; }
            set { _PeriodValue = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Strobe count slider value
        /// </summary>
        private ushort _StrobeCountValue = 0;
        public ushort StrobeCountValue
        {
            get { return _StrobeCountValue; }
            set { _StrobeCountValue = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Live control checkbox value
        /// </summary>
        private bool _LiveControl = false;
        public bool LiveControl
        {
            get { return _LiveControl; }
            set { _LiveControl = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Music control checkbox value
        /// </summary>
        private bool _MusicControl = false;
        public bool MusicControl
        {
            get { return _MusicControl; }
            set { _MusicControl = value; NotifyPropertyChanged(); }
        }

        #endregion

        public LEDGroupControl()
        {
            InitializeComponent();
        }

        #region Helper methods

        private void UpdateHsvFromRgb()
        {
            Rgb rgb = new Rgb() {
                R = ((double)this.RedValue / 257.0),
                G = ((double)this.GreenValue / 257.0),
                B = ((double)this.BlueValue / 257.0)
            };
            Hsv hsv = rgb.To<Hsv>();

            if (this.HueMaxValue != Convert.ToUInt16(hsv.H * 65536.0 / 360.0))
            {
                this._HueMaxValue = Convert.ToUInt16(hsv.H * 65536.0 / 360.0);
                if(this.HueMinValue > this._HueMaxValue)
                {
                    this._HueMinValue = this._HueMaxValue;
                    this.NotifyPropertyChanged("HueMinValue");
                }
                this.NotifyPropertyChanged("HueMaxValue");
            }

            if (this.SaturationMaxValue != Convert.ToUInt16(hsv.S * 65535.0))
            {
                this._SaturationMaxValue = Convert.ToUInt16(hsv.S * 65535.0);
                if (this.SaturationMinValue > this._SaturationMaxValue)
                {
                    this._SaturationMinValue = this._SaturationMaxValue;
                    this.NotifyPropertyChanged("HueMinValue");
                }
                this.NotifyPropertyChanged("SaturationMaxValue");
            }

            if (this.ValueMaxValue != Convert.ToUInt16(hsv.V * 65535.0))
            {
                this._ValueMaxValue = Convert.ToUInt16(hsv.V * 65535.0);
                if (this.ValueMinValue > this._ValueMaxValue)
                {
                    this._ValueMinValue = this._ValueMaxValue;
                    this.NotifyPropertyChanged("ValueMinValue");
                }
                this.NotifyPropertyChanged("ValueMaxValue");
            }

            this.NotifyPropertyChanged("ColorPickerColor");
        }

        private void UpdateRgbFromHsv()
        {
            Hsv hsv = new Hsv() {
                H = ((double)this.HueMaxValue * 360.0 / 65536.0),
                S = ((double)this.SaturationMaxValue / 65535.0),
                V = ((double)this.ValueMaxValue / 65535.0)
            };
            Rgb rgb = hsv.To<Rgb>();

            if (this.RedValue != Convert.ToUInt16(rgb.R * 257.0))
            {
                this._RedValue = Convert.ToUInt16(rgb.R * 257.0);
                this.NotifyPropertyChanged("RedValue");
            }

            if (this.GreenValue != Convert.ToUInt16(rgb.G * 257.0))
            {
                this._GreenValue = Convert.ToUInt16(rgb.G * 257.0);
                this.NotifyPropertyChanged("GreenValue");
            }

            if (this.BlueValue != Convert.ToUInt16(rgb.B * 257.0))
            {
                this._BlueValue = Convert.ToUInt16(rgb.B * 257.0);
                this.NotifyPropertyChanged("BlueValue");
            }

            this.NotifyPropertyChanged("ColorPickerColor");
        }

        #endregion
    }
}
