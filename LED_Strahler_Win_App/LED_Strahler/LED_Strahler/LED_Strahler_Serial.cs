using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Threading;

namespace LED_Strahler_GUI
{
    public class LED_Strahler_Serial
    {
        #region Properties

        private SerialPort Serial { get; set; } = null;
        private object InterfaceLock { get; set; } = new object();

        #endregion

        #region Constructor

        public LED_Strahler_Serial()
        {
            
        }

        #endregion

        #region Helper methods

        private void Write(string Text)
        {
            lock(this.InterfaceLock)
            {
                if((this.Serial != null) && (this.Serial.IsOpen == true))
                {
                    //Send data
                    var Data = Encoding.ASCII.GetBytes(Text + "\n");
                    Serial.Write(Data, 0, Data.Length);
                }
            }
        }

        private string WriteRead(string Text, int TimeoutMilliseconds, bool Wait = false)
        {
            lock(this.InterfaceLock)
            {
                if ((this.Serial != null) && (this.Serial.IsOpen == true))
                {
                    //Flush input buffer
                    Serial.DiscardInBuffer();
                    //Send data
                    var Data = Encoding.ASCII.GetBytes(Text + "\n");
                    Serial.Write(Data, 0, Data.Length);
                    //Receive data
                    if (Wait == true)
                    {
                        Thread.Sleep(TimeoutMilliseconds);
                        Text = Serial.ReadExisting();
                    }
                    else
                    {
                        Serial.ReadTimeout = TimeoutMilliseconds;
                        Text = Serial.ReadLine();
                    }
                    return Text;
                }
                return "";
            }
        }

        #endregion

        #region Methods

        public bool Open(string Port)
        {
            this.Close();

            lock(InterfaceLock)
            {
                try
                {
                    this.Serial = new SerialPort(Port, 500000, Parity.None, 8, StopBits.One);
                    this.Serial.Handshake = Handshake.None;
                    this.Serial.DtrEnable = true;
                    this.Serial.WriteTimeout = 5;
                    this.Serial.Open();
                }
                catch
                {
                    return false;
                }
                return true;
            }
        }

        public void Close()
        {
            lock(InterfaceLock)
            {
                try
                {
                    this.Serial.Close(); //Try to close port in case it was open
                }
                catch
                {
                    //Do nothing
                }
                this.Serial = null;
            }
        }

        public void PingRequest(out List<int> UUIDs)
        {
            UUIDs = new List<int>();

            string Ret = this.WriteRead("G" + (int)Commands.PingRequest, 1100, true);

            if ((Ret.Length >= 4) && (Ret.StartsWith("P: ") == true) && (Ret.Contains("\n") == true))
            {
                var Strahler = new List<string>(Ret.Replace("P: ", "").Split('\n'));
                Strahler.RemoveAt(Strahler.Count - 1); //Last element is either an empty string or corrupted data
                foreach (string ID in Strahler)
                {
                    if(int.TryParse(ID, out int UUID) == true)
                    {
                        UUIDs.Add(UUID);
                    }
                }
            }
        }

        public void SetGroup(byte GroupID, int UUID)
        {
            this.Write("G" + (int)Commands.SetGroup + " " + UUID + " " + GroupID);
        }

        //public void SetTimebase(int TimeBase)
        //{
        //    this.Write("G" + Commands.SetGroup + " " + Timebase);
        //}

        public void SetRGB(byte GroupID, ushort Red, ushort Green, ushort Blue)
        {
            this.Write("G" + (int)Commands.SetRGB + " " + GroupID + " " + Red + " " + Green + " " + Blue);
        }

        public void SetRGBW(byte GroupID, ushort Red, ushort Green, ushort Blue, ushort White)
        {
            this.Write("G" + (int)Commands.SetRGBW + " " + GroupID + " " + Red + " " + Green + " " + Blue + " " + White);
        }

        public void SetHSV(byte GroupID, ushort Hue, ushort Saturation, ushort Value)
        {
            this.Write("G" + (int)Commands.SetHSV + " " + GroupID + " " + Hue + " " + Saturation + " " + Value);
        }

        public void StrobeRGB(byte GroupID, byte Period, byte Count, ushort Red, ushort Green, ushort Blue)
        {
            this.Write("G" + (int)Commands.StrobeRGB + " " + GroupID + " " + Period + " " + Count + " " + Red + " " + Green + " " + Blue);
        }

        public void StrobeRGBW(byte GroupID, byte Period, byte Count, ushort Red, ushort Green, ushort Blue, ushort White)
        {
            this.Write("G" + (int)Commands.StrobeRGBW + " " + GroupID + " " + Period + " " + Count + " " + Red + " " + Green + " " + Blue + " " + White);
        }

        public void StrobeHSV(byte GroupID, byte Period, byte Count, ushort Hue, ushort Saturation, ushort Value)
        {
            this.Write("G" + (int)Commands.StrobeHSV + " " + GroupID + " " + Period + " " + Count + " " + Hue + " " + Saturation + " " + Value);
        }

        public void FadeHue(byte GroupID, ushort Period, ushort HueMin, ushort HueMax, ushort Saturation, ushort Value)
        {
            this.Write("G" + (int)Commands.FadeHue + " " + GroupID + " " + Period + " " + HueMax + " " + HueMin + " " + Saturation + " " + Value);
        }

        public void FadeSaturation(byte GroupID, ushort Period, ushort Hue, ushort SaturationMin, ushort SaturationMax, ushort Value)
        {
            this.Write("G" + (int)Commands.FadeSaturation + " " + GroupID + " " + Period + " " + Hue + " " + SaturationMax + " " + SaturationMin + " " + Value);
        }

        public void FadeValue(byte GroupID, ushort Period, ushort Hue, ushort Saturation, ushort ValueMin, ushort ValueMax)
        {
            this.Write("G" + (int)Commands.FadeValue + " " + GroupID + " " + Period + " " + Hue + " " + Saturation + " " + ValueMax + " " + ValueMin);
        }

        public void GetTemperature(LED_Strahler Strahler)
        {
            string Ret = this.WriteRead("G" + (int)Commands.GetTemperature + " " + Strahler.UUID, 100);

            if ((Ret.Length >= 6) && (Ret.StartsWith("T: ") == true) && (double.TryParse(Ret.Substring(3), out double Temp) == true))
            {
                Strahler.Temperature = Temp;
            }
            else
            {
                Strahler.Temperature = 0.0;
            }
        }

        #endregion
    }
}
