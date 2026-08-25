/*****************************************************************************
* | File      	:   EPD_1in54e.c
* | Author      :   Waveshare team
* | Function    :   1.54inch e-paper (E)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2026-06-29
* | Info        :
#
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
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_Test.h"
#include "EPD_1in54e.h"

int EPD_Test(void)
{
    Debug("1.54 E6 test Demo\r\n");
    DEV_Module_Init();

    Debug("e-Paper Init and Clear...\r\n");
    EPD_1IN54E_Init();
    EPD_1IN54E_Clear(EPD_1IN54E_WHITE);
    DEV_Delay_ms(1000);

    UBYTE *BlackImage;
    UDOUBLE Imagesize = ((EPD_1IN54E_WIDTH % 2 == 0)? (EPD_1IN54E_WIDTH / 2 ): (EPD_1IN54E_WIDTH / 2 + 1)) * EPD_1IN54E_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

    Debug("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_1IN54E_WIDTH, EPD_1IN54E_HEIGHT, 0, EPD_1IN54E_WHITE);
    Paint_SetScale(6);

#if 1
    Debug("show factory image--------------\r\n");
    EPD_1IN54E_Init();
    EPD_1IN54E_Display(Image6color);
    DEV_Delay_ms(3000);
#endif

#if 1
    EPD_1IN54E_Init();
    Debug("SelectImage:BlackImage\r\n");
    Paint_NewImage(BlackImage, EPD_1IN54E_WIDTH, EPD_1IN54E_HEIGHT, 0, EPD_1IN54E_WHITE);
    Paint_SetScale(6);
    Paint_SelectImage(BlackImage);
    Paint_Clear(EPD_1IN54E_WHITE);

    // 2.Drawing on the image
    Debug("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, EPD_1IN54E_RED, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, EPD_1IN54E_BLUE, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, EPD_1IN54E_GREEN, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, EPD_1IN54E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, EPD_1IN54E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, EPD_1IN54E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, EPD_1IN54E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, EPD_1IN54E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, EPD_1IN54E_WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, EPD_1IN54E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, EPD_1IN54E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_CN(10, 120, "ÄãºÃabc", &Font12CN, EPD_1IN54E_BLACK, EPD_1IN54E_WHITE);
    Paint_DrawString_CN(10, 140, "Î¢Ñ©µç×Ó", &Font24CN, EPD_1IN54E_WHITE, EPD_1IN54E_BLACK);
    Paint_DrawNum(10, 30, 123456789, &Font16, EPD_1IN54E_BLACK, EPD_1IN54E_WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, EPD_1IN54E_WHITE, EPD_1IN54E_BLACK);
    Paint_DrawString_EN(140, 30, "waveshare", &Font16, EPD_1IN54E_WHITE, EPD_1IN54E_BLACK);
    Paint_DrawString_EN(140, 50, "waveshare", &Font16, EPD_1IN54E_GREEN, EPD_1IN54E_BLACK);
    Paint_DrawString_EN(140, 70, "waveshare", &Font16, EPD_1IN54E_BLUE, EPD_1IN54E_BLACK);
    Paint_DrawString_EN(140, 90, "waveshare", &Font16, EPD_1IN54E_RED, EPD_1IN54E_BLACK);
    Paint_DrawString_EN(140, 110, "waveshare", &Font16, EPD_1IN54E_YELLOW, EPD_1IN54E_BLACK);
    Paint_DrawString_EN(140, 130, "waveshare", &Font16, EPD_1IN54E_BLACK, EPD_1IN54E_WHITE);

    Debug("EPD_Display\r\n");
    EPD_1IN54E_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

    Debug("Clear...\r\n");
    EPD_1IN54E_Init();
    EPD_1IN54E_Clear(EPD_1IN54E_WHITE);
    DEV_Delay_ms(1000);

    Debug("Goto Sleep...\r\n");
    EPD_1IN54E_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);
    Debug("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
		
	return 0;
}

