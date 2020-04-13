namespace LED_Strahler_GUI
{
    using System.Collections.Generic;
    using System.Windows.Input;
    using ColorMine.ColorSpaces;
    using System.Diagnostics;

    public partial class LEDStrahlerBackend
    {
        #region LJ releveant properties/fields

        private Hsv[] LJBaseColors = new Hsv[0];

        private bool LJUpdateLEDs = false;

        private delegate void LJhandleFunc();

        private LJhandleFunc LJhandleHue = null;

        private LJhandleFunc LJhandleSaturation = null;

        private LJhandleFunc LJhandleValue = null;

        private double LJconstanHue = 0.0;

        private int LJStrobePeriod = 0;

        private int LJStrobeSatPeriod = 0;

        private int LJFadePeriod = 0;

        private int LJFadePhase = 0;

        private readonly Stopwatch LJStrobeTimebase = new Stopwatch();

        private readonly Stopwatch LJStrobeSatTimebase = new Stopwatch();

        private readonly Stopwatch LJFadeTimebase = new Stopwatch();

        private bool LJStrobeUniform = false;

        private bool LJFadeUniform = false;

        private bool LJSetUniform = false;

        #endregion

        #region Main LJ functions

        private List<LEDStrahler> ListLJStrahler()
        {
            List<LEDStrahler> LJStrahler = new List<LEDStrahler>();
            foreach (LEDGroupControl LEDGroup in this.GUI.GroupControls)
            {
                if (LEDGroup.Backend.LJMode == true)
                {
                    if (LEDGroup.GroupID == 0)
                    {
                        return this.GUI.DeviceList; //Broadcast group includes all LEDStrahler
                    }
                    foreach (LEDStrahler Strahler in this.GUI.DeviceList)
                    {
                        if (Strahler.Group == LEDGroup.GroupID)
                        {
                            LJStrahler.Add(Strahler);
                        }
                    }
                }
            }

            return LJStrahler;
        }


        private void UpdateLJBaseColorsList(List<LEDStrahler> StrahlerListe)
        {
            if(this.LJBaseColors.Length != StrahlerListe.Count)
            {
                this.LJBaseColors = new Hsv[StrahlerListe.Count];

                this.LJUpdateLEDs = false;

                for (int i = 0; i < this.LJBaseColors.Length; i++) //Standard values for saturation and velocity
                {
                    this.LJBaseColors[i] = new Hsv();
                    this.LJBaseColors[i].H = 0.0;
                    this.LJBaseColors[i].S = 1.0;
                    this.LJBaseColors[i].V = 1.0;
                }
            }
        }


        private void LJresetHandlers()
        {
            if(this.LJhandleHue != this.LJhandleFadeHue)
            {
                this.LJhandleHue = this.LJhandleDefaultHue;
            }
            this.LJhandleSaturation = this.LJhandleDefaultSaturation;
            this.LJhandleValue = this.LJhandleDefaultValue;
        }


        private void HandleLJMode(List<LEDStrahler> StrahlerListe)
        {
            List<Key> PressedKeysClone = new List<Key>(this.PressedKeys); //Copy list

            this.UpdateLJBaseColorsList(StrahlerListe);

            //Reset handlers
            this.LJresetHandlers();

            foreach(Key Taste in PressedKeysClone)
            {
                switch(Taste)
                {
                    case Key.Q: //Set fixed color
                        this.LJSetConstantColor(0.0);
                        break;

                    case Key.W: //Set fixed color
                        this.LJSetConstantColor(36.0);
                        break;

                    case Key.E: //Set fixed color
                        this.LJSetConstantColor(72.0);
                        break;

                    case Key.R: //Set fixed color
                        this.LJSetConstantColor(108.0);
                        break;

                    case Key.T: //Set fixed color
                        this.LJSetConstantColor(144.0);
                        break;

                    case Key.Z: //Set fixed color
                        this.LJSetConstantColor(180.0);
                        break;

                    case Key.U: //Set fixed color
                        this.LJSetConstantColor(216.0);
                        break;

                    case Key.I: //Set fixed color
                        this.LJSetConstantColor(252.0);
                        break;

                    case Key.O: //Set fixed color
                        this.LJSetConstantColor(288.0);
                        break;

                    case Key.P: //Set fixed color
                        this.LJSetConstantColor(324.0);
                        break;

                    case Key.A: //Set color fading
                        this.LJFadeHue(5000);
                        break;

                    case Key.S: //Set color fading
                        this.LJFadeHue(10000);
                        break;

                    case Key.D: //Set color fading
                        this.LJFadeHue(20000);
                        break;

                    case Key.F: //Set color fading
                        this.LJFadeHue(30000);
                        break;

                    case Key.G:
                        this.LJhandleValue = this.LJhandleLEDoff;
                        break;

                    case Key.H:
                        this.LJSetStrobeSaturation(50);
                        break;

                    case Key.J:
                        this.LJSetStrobeSaturation(100);
                        break;

                    case Key.K: //Set value strobing
                        this.LJSetStrobeValue(50);
                        break;

                    case Key.L: //Set value strobing
                        this.LJSetStrobeValue(100);
                        break;

                    case Key.Y:
                        this.LJFadeUniform = false;
                        break;

                    case Key.X:
                        this.LJFadeUniform = true;
                        break;

                    case Key.C:
                        this.LJStrobeUniform = false;
                        break;

                    case Key.V:
                        this.LJStrobeUniform = true;
                        break;

                    case Key.B:
                        this.LJSetUniform = false;
                        break;

                    case Key.N:
                        this.LJSetUniform = true;
                        break;

                    case Key.M:
                        break;

                    default:
                        try
                        {
                            this.PressedKeys.Remove(Taste);
                        }
                        catch
                        {
                            //Do nothing in case of failure
                        }
                        break;
                }
            }

            //Handle fading and strobing
            this.LJhandleHue();
            this.LJhandleSaturation();
            this.LJhandleValue();

            //Actually write stuff
            //Currently only implemented in broadcast mode
            if(this.LJUpdateLEDs == true)
            {
                ushort Hue = (ushort)(65535.0 * this.LJBaseColors[0].H / 360.0);
                ushort Saturation = (ushort)(65535.0 * this.LJBaseColors[0].S);
                ushort Value = (ushort)(65535.0 * this.LJBaseColors[0].V * (double)this.GUI.GroupControls[0].Backend.GetBrightnessScale());
                LEDStrahlerSerial.SetHSV(0, Hue, Saturation, Value);
            }
        }

        #endregion

        #region LJ functions to change color and make party


        private void LJhandleDefaultHue()
        {
            this.LJFadeTimebase.Stop();
        }


        private void LJhandleDefaultSaturation()
        {
            foreach (Hsv Color in this.LJBaseColors)
            {
                Color.S = 1.0;
            }
            this.LJStrobeSatTimebase.Stop();
        }


        private void LJhandleDefaultValue()
        {
            foreach (Hsv Color in this.LJBaseColors)
            {
                Color.V = 1.0;
            }
            this.LJStrobeTimebase.Stop();
        }


        private void LJSetConstantColor(double Hue)
        {
            this.LJhandleHue = this.LJHandleConstantColor;
            this.LJconstanHue = Hue;
            this.LJUpdateLEDs = true;
        }


        private void LJHandleConstantColor()
        {
            foreach (Hsv Color in this.LJBaseColors)
            {
                Color.H = this.LJconstanHue;
                if (this.LJSetUniform == true)
                {
                    LJconstanHue += 360.0 / (double)this.LJBaseColors.Length;
                }
            }
        }


        private void LJSetStrobeValue(int Period)
        {
            if(this.LJStrobeTimebase.IsRunning == false)
            {
                this.LJStrobeTimebase.Restart();
            }
            this.LJhandleValue = this.LJhandleStrobeValue;
            this.LJStrobePeriod = Period;
            this.LJUpdateLEDs = true;
        }


        private void LJhandleStrobeValue()
        {
            int Period = (int)this.LJStrobeTimebase.ElapsedMilliseconds % this.LJStrobePeriod;

            double Value = 0.0;
            if (Period > 15 && Period < 35)
            {
                Value = 1.0;
            }

            foreach (Hsv Color in this.LJBaseColors)
            {
                Color.V = Value;
                if (this.LJStrobeUniform == true)
                {
                    Period -= this.LJStrobePeriod / this.LJBaseColors.Length;
                    Value = 0.0;
                    if (Period > 15 && Period < 35)
                    {
                        Value = 1.0;
                    }
                }
            }
        }


        private void LJSetStrobeSaturation(int Period)
        {
            if (this.LJStrobeSatTimebase.IsRunning == false)
            {
                this.LJStrobeSatTimebase.Restart();
            }
            this.LJhandleValue = this.LJhandleStrobeSaturation;
            this.LJStrobeSatPeriod = Period;
            this.LJUpdateLEDs = true;
        }


        private void LJhandleStrobeSaturation()
        {
            int Period = (int)this.LJStrobeSatTimebase.ElapsedMilliseconds % this.LJStrobeSatPeriod;

            double Value = 0.0;
            if (Period > 15 && Period < 35)
            {
                Value = 1.0;
            }

            foreach (Hsv Color in this.LJBaseColors)
            {
                Color.V = Value;
                if (this.LJStrobeUniform == true)
                {
                    Period -= this.LJStrobeSatPeriod / this.LJBaseColors.Length;
                    Value = 0.0;
                    if (Period > 15 && Period < 35)
                    {
                        Value = 1.0;
                    }
                }
            }
        }

        private void LJFadeHue(int Period)
        {
            if((this.LJFadeTimebase.IsRunning == false) || (this.LJFadePeriod != Period))
            {
                this.LJFadePhase = (int)((double)Period * this.LJBaseColors[0].H / 360.0);
                this.LJFadeTimebase.Restart();
            }
            this.LJhandleHue = this.LJhandleFadeHue;
            this.LJFadePeriod = Period;
            this.LJUpdateLEDs = true;
        }


        private void LJhandleFadeHue()
        {
            int Period = ((int)this.LJFadeTimebase.ElapsedMilliseconds + this.LJFadePhase ) % this.LJFadePeriod;

            double Hue = 360.0 * (double)Period / (double)this.LJFadePeriod;

            foreach (Hsv Color in this.LJBaseColors)
            {
                Color.H = Hue;
                if (this.LJFadeUniform == true)
                {
                    Hue += 360.0 / (double)this.LJBaseColors.Length;
                }
            }
        }

        
        private void LJhandleLEDoff()
        {
            foreach (Hsv Color in this.LJBaseColors)
            {
                Color.V = 0.0;
            }
        }

        #endregion
    }
}
