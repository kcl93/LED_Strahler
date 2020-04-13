namespace LED_Strahler_GUI
{
    using System;
    using System.Windows;
    using ColorMine.ColorSpaces;
    using System.ComponentModel;
    using System.Windows.Media;
    using System.Runtime.CompilerServices;

    public class LEDGroupControl_Backend : INotifyPropertyChanged
    {
        #region Property changed

        public event PropertyChangedEventHandler PropertyChanged;

        // This method is called by the Set accessor of each property.  
        // The CallerMemberName attribute that is applied to the optional propertyName  
        // parameter causes the property name of the caller to be substituted as an argument.  
        public void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        #endregion

        #region Properties

        private LEDGroupControl GUI { get; set; } = null;

        /// <summary>
        /// Group name of the control
        /// </summary>
        public string Group
        {
            get
            {
                if (GroupID == 0)
                {
                    return "Broadcast";
                }
                return "Group " + GroupID;
            }
        }

        /// <summary>
        /// Group ID of the group control
        /// </summary>
        private byte _GroupID = 0;
        public byte GroupID
        {
            get { return _GroupID; }
            set { _GroupID = value; NotifyPropertyChanged(); NotifyPropertyChanged("Group"); }
        }

        /// <summary>
        /// Color from the color picker
        /// </summary>
        public Color ColorPickerColor
        {
            get { return Color.FromRgb(Convert.ToByte(RedValue / 257.0), Convert.ToByte(GreenValue / 257.0), Convert.ToByte(BlueValue / 257.0)); }
            set
            {
                if ((value.R != Convert.ToByte(RedValue / 257.0)) || (value.G != Convert.ToByte(GreenValue / 257.0)) || (value.B != Convert.ToByte(BlueValue / 257.0)))
                {
                    RedValue = (ushort)(257 * value.R);
                    GreenValue = (ushort)(257 * value.G);
                    BlueValue = (ushort)(257 * value.B);
                    NotifyPropertyChanged();
                    this.UpdateHsvFromRgb();
                }
            }
        }

        /// <summary>
        /// Red color value
        /// </summary>
        private ushort _RedValue = 65535;
        public ushort RedValue
        {
            get { return _RedValue; }
            set
            {
                if (_RedValue != value)
                {
                    _RedValue = value;
                    NotifyPropertyChanged();
                    this.UpdateHsvFromRgb();
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
                    this.UpdateHsvFromRgb();
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
                if (_BlueValue != value)
                {
                    _BlueValue = value;
                    NotifyPropertyChanged();
                    this.UpdateHsvFromRgb();
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
                if (_HueMinValue != value)
                {
                    if (_HueMaxValue < value)
                    {
                        HueMaxValue = value;
                    }
                    _HueMinValue = value;
                    NotifyPropertyChanged();
                    if(this.GUI.RB_FadeButton.IsChecked == true)
                    {
                        DoLiveControlUpdate();
                    }
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
                if (_HueMaxValue != value)
                {
                    _HueMaxValue = value;
                    if (_HueMinValue > value)
                    {
                        HueMinValue = value;
                    }
                    NotifyPropertyChanged();
                    this.UpdateRgbFromHsv();
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
                if (_SaturationMinValue != value)
                {
                    if (_SaturationMaxValue < value)
                    {
                        SaturationMaxValue = value;
                    }
                    _SaturationMinValue = value;
                    NotifyPropertyChanged();
                    if (this.GUI.RB_FadeButton.IsChecked == true)
                    {
                        DoLiveControlUpdate();
                    }
                }
            }
        }

        /// <summary>
        /// Saturation max slider value
        /// </summary>
        private ushort _SaturationMaxValue = 65535;
        public ushort SaturationMaxValue
        {
            get { return _SaturationMaxValue; }
            set
            {
                if (_SaturationMaxValue != value)
                {
                    _SaturationMaxValue = value;
                    if (_SaturationMinValue > value)
                    {
                        SaturationMinValue = value;
                    }
                    NotifyPropertyChanged();
                    this.UpdateRgbFromHsv();
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
                if (_ValueMinValue != value)
                {
                    if (_ValueMaxValue < value)
                    {
                        ValueMaxValue = value;
                    }
                    _ValueMinValue = value;
                    NotifyPropertyChanged();
                    if (this.GUI.RB_FadeButton.IsChecked == true)
                    {
                        DoLiveControlUpdate();
                    }
                }
            }
        }

        /// <summary>
        /// Value max slider value
        /// </summary>
        private ushort _ValueMaxValue = 65535;
        public ushort ValueMaxValue
        {
            get { return _ValueMaxValue; }
            set
            {
                if (_ValueMaxValue != value)
                {
                    _ValueMaxValue = value;
                    if (_ValueMinValue > value)
                    {
                        ValueMinValue = value;
                    }
                    NotifyPropertyChanged();
                    this.UpdateRgbFromHsv();
                }
            }
        }

        /// <summary>
        /// Global brightness slider value
        /// </summary>
        private ushort _BrightnessValue = 10000;
        public ushort BrightnessValue
        {
            get { return _BrightnessValue; }
            set { _BrightnessValue = value; NotifyPropertyChanged(); DoLiveControlUpdate(); }
        }

        /// <summary>
        /// Period slider value for strobes and fading
        /// </summary>
        private ushort _PeriodValue = 16384;
        public ushort PeriodValue
        {
            get { return _PeriodValue; }
            set
            {
                _PeriodValue = value;
                NotifyPropertyChanged();
                if (this.GUI.RB_SetButton.IsChecked == false) //If the set button is not checked, the fade or strobe ones must be
                {
                    DoLiveControlUpdate();
                }
            }
        }

        /// <summary>
        /// Strobe count slider value
        /// </summary>
        private ushort _StrobeCountValue = 5000;
        public ushort StrobeCountValue
        {
            get { return _StrobeCountValue; }
            set { _StrobeCountValue = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Static control radiobutton value
        /// </summary>
        private bool _StaticControl = true;
        public bool StaticControl
        {
            get { return _StaticControl; }
            set { _StaticControl = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Live control radiobutton value
        /// </summary>
        private bool _LiveControl = false;
        public bool LiveControl
        {
            get { return _LiveControl; }
            set { _LiveControl = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Music control radiobutton value
        /// </summary>
        private bool _MusicControl = false;
        public bool MusicControl
        {
            get { return _MusicControl; }
            set { _MusicControl = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// LJ mode radiobutton value
        /// </summary>
        private bool _LJMode = false;
        public bool LJMode
        {
            get { return _LJMode; }
            set { _LJMode = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Candle mode radiobutton value
        /// </summary>
        private bool _CandleMode = false;
        public bool CandleMode
        {
            get { return _CandleMode; }
            set { _CandleMode = value; NotifyPropertyChanged(); }
        }

        /// <summary>
        /// Enable buttons value
        /// </summary>
        private bool _EnableButtons = true;
        public bool EnableButtons
        {
            get { return _EnableButtons; }
            set { _EnableButtons = value; NotifyPropertyChanged(); }
        }

        #endregion

        #region Constructor

        public LEDGroupControl_Backend()
        {

        }

        #endregion

        #region Helper methods

        public void LinkGUI(LEDGroupControl _gui)
        {
            this.GUI = _gui;

            this.GUI.DataContext = this;

            this.GroupID = this.GUI.GroupID;

            this.GUI.SetButton.Click += this.SetButtonClick;
            this.GUI.FadeButton.Click += this.FadeButtonClick;
            this.GUI.StrobeButton.Click += this.StrobeButtonClick;
        }

        public void DoLiveControlUpdate()
        {
            if (this.LiveControl == true)
            {
                if (this.GUI.RB_SetButton.IsChecked == true)
                {
                    SetButtonClick(null, null);
                }
                else if (this.GUI.RB_FadeButton.IsChecked == true)
                {
                    FadeButtonClick(null, null);
                }
                else if (this.GUI.RB_StrobeButton.IsChecked == true)
                {
                    StrobeButtonClick(null, null);
                }
            }
        }

        public double GetBrightnessScale()
        {
            return Math.Pow((double)this.BrightnessValue, 2) / Math.Pow(65535, 2);
        }

        public void SetButtonClick(object sender, RoutedEventArgs e)
        {
            double BrightnessScaling = GetBrightnessScale();
            ushort Red = Convert.ToUInt16((double)this.RedValue * BrightnessScaling);
            ushort Green = Convert.ToUInt16((double)this.GreenValue * BrightnessScaling);
            ushort Blue = Convert.ToUInt16((double)this.BlueValue * BrightnessScaling);
            LEDStrahlerSerial.SetRGB(this.GroupID, Red, Green, Blue);
        }

        public void StrobeButtonClick(object sender, RoutedEventArgs e)
        {
            double BrightnessScaling = GetBrightnessScale();
            ushort Red = Convert.ToUInt16((double)this.RedValue * BrightnessScaling);
            ushort Green = Convert.ToUInt16((double)this.GreenValue * BrightnessScaling);
            ushort Blue = Convert.ToUInt16((double)this.BlueValue * BrightnessScaling);
            byte Period = Convert.ToByte(Math.Pow((double)this.PeriodValue / 257, 2) / 255);
            byte StrobeCount = Convert.ToByte(Math.Pow((double)this.StrobeCountValue / 257, 1.5) / Math.Pow(255, 0.5));
            LEDStrahlerSerial.StrobeRGB(this.GroupID, Period, StrobeCount, Red, Green, Blue);
        }

        public void FadeButtonClick(object sender, RoutedEventArgs e)
        {
            double BrightnessScaling = GetBrightnessScale();
            ushort Period = Convert.ToUInt16(Math.Pow((double)this.PeriodValue, 2) / 65535);
            if (this.GUI.RB_Hue.IsChecked == true)
            {
                ushort Value = Convert.ToUInt16((double)this.ValueMaxValue * BrightnessScaling);
                LEDStrahlerSerial.FadeHue(this.GroupID, Period, this.HueMinValue, this.HueMaxValue, this.SaturationMaxValue, Value);
            }
            else if (this.GUI.RB_Saturation.IsChecked == true)
            {
                ushort Value = Convert.ToUInt16((double)this.ValueMaxValue * BrightnessScaling);
                LEDStrahlerSerial.FadeSaturation(this.GroupID, Period, this.HueMaxValue, this.SaturationMinValue, this.SaturationMaxValue, Value);
            }
            else if (this.GUI.RB_Value.IsChecked == true)
            {
                ushort ValueMax = Convert.ToUInt16((double)this.ValueMaxValue * BrightnessScaling);
                ushort ValueMin = Convert.ToUInt16((double)this.ValueMinValue * BrightnessScaling);
                LEDStrahlerSerial.FadeValue(this.GroupID, Period, this.HueMaxValue, this.SaturationMaxValue, ValueMin, ValueMax);
            }
        }

        public void UpdateHsvFromRgb()
        {
            Rgb rgb = new Rgb()
            {
                R = ((double)this.RedValue / 257.0),
                G = ((double)this.GreenValue / 257.0),
                B = ((double)this.BlueValue / 257.0)
            };
            Hsv hsv = rgb.To<Hsv>();

            if (this.HueMaxValue != Convert.ToUInt16(hsv.H * 65536.0 / 360.0))
            {
                this.HueMaxValue = Convert.ToUInt16(hsv.H * 65536.0 / 360.0);
                if (this.HueMinValue > this._HueMaxValue)
                {
                    this.HueMinValue = this._HueMaxValue;
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
            DoLiveControlUpdate();
        }

        public void UpdateRgbFromHsv()
        {
            Hsv hsv = new Hsv()
            {
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
            DoLiveControlUpdate();
        }


        #endregion
    }
}
