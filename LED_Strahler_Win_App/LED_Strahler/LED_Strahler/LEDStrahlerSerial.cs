namespace LED_Strahler_GUI
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.IO.Ports;
    using System.Threading;
    using System.Globalization;
    using System.Windows.Threading;

    public static class LEDStrahlerSerial
    {
        #region Properties

        private static SerialPort Serial { get; set; } = null;
        private static object InterfaceLock { get; set; } = new object();
        private static Dispatcher ThreadDispatcher { get; set; } = null;
        private static Thread BackgroundThread { get; set; } = null;
        private static bool KeepAlive { get; set; } = true;

        #endregion

        #region Helper methods

        private static void RunDispatcher()
        {
            while(LEDStrahlerSerial.KeepAlive)
            {
                try
                {
                    //Create dispatcher for thread
                    LEDStrahlerSerial.ThreadDispatcher = Dispatcher.CurrentDispatcher;

                    //Create serial object
                    LEDStrahlerSerial.Serial = new SerialPort();
                    LEDStrahlerSerial.Serial.BaudRate = 500000;
                    LEDStrahlerSerial.Serial.Parity = Parity.None;
                    LEDStrahlerSerial.Serial.DataBits = 8;
                    LEDStrahlerSerial.Serial.StopBits = StopBits.One;
                    LEDStrahlerSerial.Serial.Handshake = Handshake.None;
                    LEDStrahlerSerial.Serial.DtrEnable = true;
                    LEDStrahlerSerial.Serial.WriteTimeout = 5;

                    //Run dispatcher
                    Dispatcher.Run();
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex);
                }
            }
        }

        private static void WriteInternal(string Text)
        {
            lock(LEDStrahlerSerial.InterfaceLock)
            {
                if(LEDStrahlerSerial.Serial.IsOpen == true)
                {
                    //Send data
                    var Data = Encoding.ASCII.GetBytes(Text + "\n");
                    Serial.Write(Data, 0, Data.Length);
                }
            }
        }

        private static string WriteReadInternal(string Text, int TimeoutMilliseconds, bool Wait = false)
        {
            lock(LEDStrahlerSerial.InterfaceLock)
            {
                if (LEDStrahlerSerial.Serial.IsOpen == true)
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
                        try
                        {
                            Text = Serial.ReadLine();
                        }
                        catch
                        {
                            return ""; //Timeout
                        }
                    }
                    return Text;
                }
                return "";
            }
        }

        private static bool OpenInternal(string Port)
        {
            LEDStrahlerSerial.CloseInternal();

            lock (InterfaceLock)
            {
                try
                {
                    LEDStrahlerSerial.Serial.PortName = Port;
                    LEDStrahlerSerial.Serial.Open();
                }
                catch
                {
                    return false;
                }
                return true;
            }
        }

        private static void CloseInternal()
        {
            lock (InterfaceLock)
            {
                try
                {
                    LEDStrahlerSerial.Serial.Close(); //Try to close port in case it was open
                }
                catch
                {
                    //Do nothing
                }
            }
        }

        private static void Write(string Text)
        {
            LEDStrahlerSerial.ThreadDispatcher.BeginInvoke(new Action(() => LEDStrahlerSerial.WriteInternal(Text)));
        }

        private static string WriteRead(string Text, int TimeoutMilliseconds, bool Wait = false)
        {
            string retVal = "";
            LEDStrahlerSerial.ThreadDispatcher.Invoke(new Action(() => retVal = LEDStrahlerSerial.WriteReadInternal(Text, TimeoutMilliseconds, Wait)));
            return retVal;
        }

        #endregion

        #region Methods

        public static void StartDispatcher()
        {
            LEDStrahlerSerial.BackgroundThread = new Thread(LEDStrahlerSerial.RunDispatcher);
            LEDStrahlerSerial.BackgroundThread.Start();
        }

        public static void ShutdownDispatcher()
        {
            try
            {
                LEDStrahlerSerial.KeepAlive = false;
                LEDStrahlerSerial.ThreadDispatcher.InvokeShutdown();
                Thread.Sleep(100);
                LEDStrahlerSerial.BackgroundThread.Abort();
            }
            catch
            {
                //Do nothing
            }
        }

        public static List<string> ListPorts()
        {
            return new List<string>(SerialPort.GetPortNames());
        }

        public static bool Open(string Port)
        {
            bool retVal = false;
            LEDStrahlerSerial.ThreadDispatcher.Invoke(new Action(() => retVal = LEDStrahlerSerial.OpenInternal(Port)));
            return retVal;
        }

        public static void Close()
        {
            LEDStrahlerSerial.ThreadDispatcher.Invoke(new Action(() => LEDStrahlerSerial.Close()));
        }

        public static void PingRequest(out List<uint> UUIDs)
        {
            UUIDs = new List<uint>();

            string Ret = LEDStrahlerSerial.WriteRead("G" + (int)Commands.PingRequest, 1100, true);

            if ((Ret.Length >= 4) && (Ret.StartsWith("P: ") == true) && (Ret.Contains("\n") == true))
            {
                var Strahler = new List<string>(Ret.Replace("P: ", "").Split('\n'));
                Strahler.RemoveAt(Strahler.Count - 1); //Last element is either an empty string or corrupted data
                foreach (string ID in Strahler)
                {
                    if(uint.TryParse(ID, out uint UUID) == true)
                    {
                        if(UUIDs.Contains(UUID) == false)
                        {
                            UUIDs.Add(UUID);
                        }
                    }
                }
            }
        }

        public static void SetGroup(byte GroupID, uint UUID)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.SetGroup + " " + UUID + " " + GroupID);
        }

        //public void SetTimebase(int TimeBase)
        //{
        //    LEDStrahlerSerial.Write("G" + Commands.SetGroup + " " + Timebase);
        //}

        public static void SetRGB(byte GroupID, ushort Red, ushort Green, ushort Blue)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.SetRGB + " " + GroupID + " " + Red + " " + Green + " " + Blue);
        }

        public static void SetRGB(LEDStrahler Strahler, ushort Red, ushort Green, ushort Blue)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.SetRGB + " 0 " + Red + " " + Green + " " + Blue + " " + Strahler.UUID);
        }

        public static void SetRGBW(byte GroupID, ushort Red, ushort Green, ushort Blue, ushort White)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.SetRGBW + " " + GroupID + " " + Red + " " + Green + " " + Blue + " " + White);
        }

        public static void SetRGBW(LEDStrahler Strahler, ushort Red, ushort Green, ushort Blue, ushort White)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.SetRGBW + " 0 " + Red + " " + Green + " " + Blue + " " + White + " " + Strahler.UUID);
        }

        public static void SetHSV(byte GroupID, ushort Hue, ushort Saturation, ushort Value)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.SetHSV + " " + GroupID + " " + Hue + " " + Saturation + " " + Value);
        }

        public static void SetHSV(LEDStrahler Strahler, ushort Hue, ushort Saturation, ushort Value)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.SetHSV + " 0 " + Hue + " " + Saturation + " " + Value + " " + Strahler.UUID);
        }

        public static void StrobeRGB(byte GroupID, byte Period, byte Count, ushort Red, ushort Green, ushort Blue)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.StrobeRGB + " " + GroupID + " " + Period + " " + Count + " " + Red + " " + Green + " " + Blue);
        }

        public static void StrobeRGB(LEDStrahler Strahler, byte Period, byte Count, ushort Red, ushort Green, ushort Blue)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.StrobeRGB + " 0 " + Period + " " + Count + " " + Red + " " + Green + " " + Blue + " " + Strahler.UUID);
        }

        public static void StrobeRGBW(byte GroupID, byte Period, byte Count, ushort Red, ushort Green, ushort Blue, ushort White)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.StrobeRGBW + " " + GroupID + " " + Period + " " + Count + " " + Red + " " + Green + " " + Blue + " " + White);
        }

        public static void StrobeRGBW(LEDStrahler Strahler, byte Period, byte Count, ushort Red, ushort Green, ushort Blue, ushort White)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.StrobeRGBW + " 0 " + Period + " " + Count + " " + Red + " " + Green + " " + Blue + " " + White + " " + Strahler.UUID);
        }

        public static void StrobeHSV(byte GroupID, byte Period, byte Count, ushort Hue, ushort Saturation, ushort Value)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.StrobeHSV + " " + GroupID + " " + Period + " " + Count + " " + Hue + " " + Saturation + " " + Value);
        }

        public static void StrobeHSV(LEDStrahler Strahler, byte Period, byte Count, ushort Hue, ushort Saturation, ushort Value)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.StrobeHSV + " 0 " + Period + " " + Count + " " + Hue + " " + Saturation + " " + Value + " " + Strahler.UUID);
        }

        public static void FadeHue(byte GroupID, ushort Period, ushort HueMin, ushort HueMax, ushort Saturation, ushort Value)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.FadeHue + " " + GroupID + " " + Period + " " + HueMax + " " + HueMin + " " + Saturation + " " + Value);
        }

        public static void FadeHue(LEDStrahler Strahler, ushort Period, ushort HueMin, ushort HueMax, ushort Saturation, ushort Value)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.FadeHue + " 0 " + Period + " " + HueMax + " " + HueMin + " " + Saturation + " " + Value + " " + Strahler.UUID);
        }

        public static void FadeSaturation(byte GroupID, ushort Period, ushort Hue, ushort SaturationMin, ushort SaturationMax, ushort Value)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.FadeSaturation + " " + GroupID + " " + Period + " " + Hue + " " + SaturationMax + " " + SaturationMin + " " + Value);
        }

        public static void FadeSaturation(LEDStrahler Strahler, ushort Period, ushort Hue, ushort SaturationMin, ushort SaturationMax, ushort Value)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.FadeSaturation + " 0 " + Period + " " + Hue + " " + SaturationMax + " " + SaturationMin + " " + Value + " " + Strahler.UUID);
        }

        public static void FadeValue(byte GroupID, ushort Period, ushort Hue, ushort Saturation, ushort ValueMin, ushort ValueMax)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.FadeValue + " " + GroupID + " " + Period + " " + Hue + " " + Saturation + " " + ValueMax + " " + ValueMin);
        }

        public static void FadeValue(LEDStrahler Strahler, ushort Period, ushort Hue, ushort Saturation, ushort ValueMin, ushort ValueMax)
        {
            LEDStrahlerSerial.Write("G" + (int)Commands.FadeValue + " 0 " + Period + " " + Hue + " " + Saturation + " " + ValueMax + " " + ValueMin + " " + Strahler.UUID);
        }

        public static void GetTemperature(LEDStrahler Strahler)
        {
            string Ret = LEDStrahlerSerial.WriteRead("G" + (int)Commands.GetTemperature + " " + Strahler.UUID, 200);
            
            if ((Ret.Length >= 6) && (Ret.StartsWith("T: ") == true) && (double.TryParse(Ret.Substring(3), NumberStyles.Number, CultureInfo.InvariantCulture, out double Temp) == true))
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
