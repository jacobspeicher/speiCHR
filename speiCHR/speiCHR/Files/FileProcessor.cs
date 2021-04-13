using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Runtime.CompilerServices;
using System.ComponentModel;

namespace speiCHR.Files
{
    class FileProcessor
    {
        private BinaryReader reader;
        private CHRFile chrFile;

        public FileProcessor(CHRFile f)
        {
            chrFile = f;
        }

        public string ReadFiles(string file)
        {
            chrFile.Name = file.Substring(file.LastIndexOf("\\") + 1);
            reader = new BinaryReader(File.Open(file, FileMode.Open));

            byte[] bits = reader.ReadBytes(16);

            string output = "";
            for(int i = 0; i < 8; ++i)
            {
                string row = "";
                for(int j = 0; j < 8; ++j)
                {
                    int lo_bit = bits[i] & 1;
                    int hi_bit = bits[i + 8] & 1;
                    bits[i] = (byte)(bits[i] >> 1);
                    bits[i + 8] = (byte)(bits[i + 8] >> 1);
                    row = lo_bit.ToString() + hi_bit.ToString() + " " + row;
                }
                output += row + "\n";
            }

            return output;
        }
    }
}
