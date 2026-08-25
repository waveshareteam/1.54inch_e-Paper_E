# *****************************************************************************
# * | File        :   epd1in54e.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V1.0
# * | Date        :   2026-06-29
# # | Info        :   python demo
# -----------------------------------------------------------------------------
# ******************************************************************************/
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

import logging
from . import epdconfig

import PIL
from PIL import Image
import io

# Display resolution
EPD_WIDTH       = 240
EPD_HEIGHT      = 240

logger = logging.getLogger(__name__)

class EPD:
    def __init__(self):
        self.reset_pin = epdconfig.RST_PIN
        self.dc_pin = epdconfig.DC_PIN
        self.busy_pin = epdconfig.BUSY_PIN
        self.cs_pin = epdconfig.CS_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT
        self.BLACK  = 0x000000   #   0000  BGR
        self.WHITE  = 0xffffff   #   0001
        self.YELLOW = 0x00ffff   #   0010
        self.RED    = 0x0000ff   #   0011
        self.BLUE   = 0xff0000   #   0101
        self.GREEN  = 0x00ff00   #   0110
        

    # Hardware reset
    def reset(self):
        epdconfig.digital_write(self.reset_pin, 0)
        epdconfig.delay_ms(20)
        epdconfig.digital_write(self.reset_pin, 1)
        epdconfig.delay_ms(20)
        self.ReadBusyH()
        epdconfig.delay_ms(10)

    def send_command(self, command):
        epdconfig.digital_write(self.dc_pin, 0)
        epdconfig.digital_write(self.cs_pin, 0)
        epdconfig.spi_writebyte([command])
        epdconfig.digital_write(self.cs_pin, 1)

    def send_data(self, data):
        epdconfig.digital_write(self.dc_pin, 1)
        epdconfig.digital_write(self.cs_pin, 0)
        epdconfig.spi_writebyte([data])
        epdconfig.digital_write(self.cs_pin, 1)
        
    # send a lot of data   
    def send_data2(self, data):
        epdconfig.digital_write(self.dc_pin, 1)
        epdconfig.digital_write(self.cs_pin, 0)
        epdconfig.spi_writebyte2(data)
        epdconfig.digital_write(self.cs_pin, 1)
        
    def ReadBusyH(self):
        logger.debug("e-Paper busy H")
        while(epdconfig.digital_read(self.busy_pin) == 0):      # 0: busy, 1: idle
            epdconfig.delay_ms(10)
        epdconfig.delay_ms(20)
        logger.debug("e-Paper busy H release")

    def TurnOnDisplay(self):
        self.send_command(0x04) # POWER_ON
        self.ReadBusyH()
        epdconfig.delay_ms(10)

        self.send_command(0x12) # DISPLAY_REFRESH
        self.send_data(0x00)
        epdconfig.delay_ms(10)
        self.ReadBusyH()
        
        self.send_command(0x02) # POWER_OFF
        self.send_data(0x00)
        self.ReadBusyH()
        epdconfig.delay_ms(20)
        
    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        self.reset()
        self.ReadBusyH()

        self.send_command(0xE9)
        self.send_data(0x01)
        return 0

    def getbuffer(self, image):
        # Create a pallette with the 7 colors supported by the panel
        pal_image = Image.new("P", (1,1))
        pal_image.putpalette( (0,0,0,  255,255,255,  255,255,0,  255,0,0,  0,0,0,  0,0,255,  0,255,0) + (0,0,0)*249)

        # Check if we need to rotate the image
        imwidth, imheight = image.size
        if(imwidth == self.width and imheight == self.height):
            image_temp = image
        elif(imwidth == self.height and imheight == self.width):
            image_temp = image.rotate(90, expand=True)
        else:
            logger.warning("Invalid image dimensions: %d x %d, expected %d x %d" % (imwidth, imheight, self.width, self.height))

        # Convert the soruce image to the 7 colors, dithering if needed
        image_6color = image_temp.convert("RGB").quantize(palette=pal_image)
        buf_6color = bytearray(image_6color.tobytes('raw'))

        # PIL does not support 4 bit color, so pack the 4 bits of color
        # into a single byte to transfer to the panel
        buf = [0x00] * int(self.width * self.height / 2)
        idx = 0
        for i in range(0, len(buf_6color), 2):
            buf[idx] = (buf_6color[i] << 4) + buf_6color[i+1]
            idx += 1
            
        return buf

    def get_color_code(self, color):
        if(color == self.BLACK):
            return 0x0
        if(color == self.WHITE):
            return 0x1
        if(color == self.YELLOW):
            return 0x2
        if(color == self.RED):
            return 0x3
        if(color == self.BLUE):
            return 0x5
        if(color == self.GREEN):
            return 0x6
        return color & 0x0F

    def display(self, image):
        self.send_command(0x10)
        self.send_data2(image)

        self.TurnOnDisplay()
        
    def Clear(self, color=0x1):
        color = self.get_color_code(color)
        self.send_command(0x10)
        self.send_data2([((color << 4) | color)] * int(self.height) * int((self.width + 1) / 2))

        self.TurnOnDisplay()

    def sleep(self):
        self.send_command(0x07) # DEEP_SLEEP
        self.send_data(0XA5)
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
### END OF FILE ###
