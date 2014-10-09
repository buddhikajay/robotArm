﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;

namespace RobotInteractive_Windows
{
    public class SerialConnector
    {
        SerialPort ComPort = new SerialPort();
        SerialPort ConnectedPort;
        string[] PortNames = null;
        
        public void scanComPorts(ComboBox ComboPorts)
        {
            int index = -1;
            string PortNm = null;

            PortNames = SerialPort.GetPortNames();
            Array.Sort(PortNames);

            try
            {
                do
                {
                    index += 1;
                    ComboPorts.Items.Add(PortNames[index]);
                    ComboPorts.Text = PortNames[0];
                } while (!((PortNames[index] == PortNm)) || (index == PortNames.GetUpperBound(0)));

            }
            catch (IndexOutOfRangeException)
            {
                MessageBox.Show("Reading COM ports");
            }

        }


        public SerialPort connectRobot(string portName)
        {
            //selected values according to robots Atmega2560
            ConnectedPort = new SerialPort(portName, 9600, Parity.None, 8, StopBits.One);
            return ConnectedPort;
        }
        
    }

    class Interpreter
    {
        public int[] interpret(string command)
        {
            int[] roboCommand = new int[3];
            char motorID;
            int position1 = 0;//1st byte of position
            int position2 = 0;//2nd byte of position
            int position = 0;


            string[] sections;

            sections = command.Split(' ');
            motorID = sections[1][0];
            Debug.WriteLine(motorID);
            try
            {
                if (sections[2].Equals("STOP"))
                {
                    motorID = (char)(motorID ^ 32);
                }
                else
                {
                    try
                    {
                        position = Convert.ToInt16(sections[2]);
                    }
                    catch (System.FormatException e)
                    {
                        MessageBox.Show("Use correct format\nEx: Moter A 32547", "Invalid Command",
               MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                    if (position < 0)
                    {
                        //if position is negative add 1 to 8th bit
                        Debug.WriteLine(position);
                        position *= (-1);
                        position1 += (1 << 7);
                    }

                    position1 += (position / 255);
                    position2 = (position % 255);

                }
            }
            catch (System.IndexOutOfRangeException e)
            {
                Debug.WriteLine(e);
                MessageBox.Show("Use correct format\nEx: Moter A STOP", "Invalid Command",
                MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            Debug.WriteLine(position1);
            Debug.WriteLine(position2);
            roboCommand[0] = (int)motorID;
            roboCommand[1] = position1;
            roboCommand[2] = position2;

            return roboCommand;
        }

        public void divideIntoLines( RichTextBox rtbCode )
        {
            //LineNum
            string readCode = rtbCode.Text;
            StreamReader sd = new StreamReader(readCode);
            Queue<string> code = new Queue<string>();
            for (int i = 0; i < rtbCode.Lines.Length; i++)
            {
                string line = sd.ReadLine();
                code.Enqueue(line);
                Debug.WriteLine(line);
                //sw.WriteLine(rtbCode.Lines[i]);
            }
    

        }


    }//Interpreter Class
}
