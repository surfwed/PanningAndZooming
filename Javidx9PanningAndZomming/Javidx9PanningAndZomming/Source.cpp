#include"olcConsoleGameEngine.h"
#include<string>
using namespace std;
class OneLoneCoder_PanAndZoom : public olcConsoleGameEngine
{
public:
	OneLoneCoder_PanAndZoom()
	{
		m_sAppName = L"Pan And Zoom";
	}
private:
	float fOffsetX = 0.0f;
	float fOffsetY = 0.0f;

	float fStartPanX = 0.0f;
	float fStartPanY = 0.0f;

	// zomming required the scale
	float fScaleX = 1.0f;
	float fScaleY = 1.0f;

	float fSelectedCellX = 0.0f;
	float fSelectedCellY = 0.0f;

	void WorldToScreen(float fWorldX, float fWorldY, int &nScreenX, int &nScreenY)
	{
		nScreenX = (int)((fWorldX - fOffsetX)*fScaleX);
		nScreenY = (int)((fWorldY - fOffsetY)*fScaleY);
	}
	void ScreenToWorld(int nScreenX, int nScreenY, float & fWorldX, float & fWorldY)
	{
		fWorldX = (float)(nScreenX) / fScaleX + fOffsetX;
		fWorldY = (float)(nScreenY) / fScaleY + fOffsetY;
	}

	
protected:
	virtual bool OnUserCreate()
	{
		// Khoi tao offset world space o giua man hinh
		fOffsetX = -ScreenWidth() / 2;
		fOffsetY = -ScreenHeight() / 2;
		return true;
	}
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		// grab a copy of mouse coordinate 
		float fMouseX = (float)GetMouseX();
		float fMouseY = (float)GetMouseY();

		if (GetMouse(0).bPressed)
		{
			fStartPanX = fMouseX;
			fStartPanY = fMouseY;
		}
		
		if (GetMouse(0).bHeld)
		{
			fOffsetX -= (fMouseX - fStartPanX) / fScaleX;
			fOffsetY -= (fMouseY - fStartPanY) / fScaleY;


			fStartPanX = fMouseX;
			fStartPanY = fMouseY;
		}

		float fMouseWorldX_BeforeZoom, fMouseWorldY_BeforeZoom;
		ScreenToWorld(fMouseX, fMouseY, fMouseWorldX_BeforeZoom, fMouseWorldY_BeforeZoom);
		// Zoom in
		if (GetKey(L'Q').bHeld)
		{
			fScaleX *= 1.01f;
			fScaleY *= 1.01f;
		}	
		// Zoom out
		if (GetKey(L'A').bHeld)
		{
			fScaleX *= 0.99f;
			fScaleY *= 0.99f;
		}
		float fMouseWorldX_AfterZoom, fMouseWorldY_AfterZoom;
		ScreenToWorld(fMouseX, fMouseY, fMouseWorldX_AfterZoom, fMouseWorldY_AfterZoom);
		fOffsetX -= (fMouseWorldX_AfterZoom - fMouseWorldX_BeforeZoom);
		fOffsetY -= (fMouseWorldY_AfterZoom - fMouseWorldY_BeforeZoom);

		if (GetMouse(1).bReleased)
		{
			fSelectedCellX = (int)fMouseWorldX_AfterZoom;
			fSelectedCellY = (int)fMouseWorldY_AfterZoom;
		}

		// Clear screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		float fWorldLeft, fWorldTop, fWorldRight, fWorldBottom;
		ScreenToWorld(0, 0, fWorldLeft, fWorldTop);
		ScreenToWorld(ScreenWidth(), ScreenHeight(), fWorldRight, fWorldBottom);
		int nLinesDrawn = 0;
		// Draw horizontal line
		for (float y = 0.0f; y <= 10.0f; y++)
		{
			if (y >= fWorldTop && y <= fWorldBottom)
			{
			float sx = 0.0f, sy = y;
			float ex = 10.0f, ey = y;

			int pixel_sx, pixel_sy, pixel_ex, pixel_ey;

			WorldToScreen(sx, sy, pixel_sx, pixel_sy);
			WorldToScreen(ex, ey, pixel_ex, pixel_ey);

			DrawLine(pixel_sx, pixel_sy, pixel_ex, pixel_ey, PIXEL_SOLID, FG_WHITE);
			nLinesDrawn++;
			}
		}

		// Draw vertical line
		for (float x = 0.0f; x <= 10.0f; x++)
		{
			if (x >= fWorldLeft && x <= fWorldRight)
			{
			float sx = x, sy = 0.0f;
			float ex = x, ey = 10.0f;

			int pixel_sx, pixel_sy, pixel_ex, pixel_ey;

			WorldToScreen(sx, sy, pixel_sx, pixel_sy);
			WorldToScreen(ex, ey, pixel_ex, pixel_ey);

			DrawLine(pixel_sx, pixel_sy, pixel_ex, pixel_ey, PIXEL_SOLID, FG_WHITE);
			nLinesDrawn++;
			}
		}		

		int cx, cy, cr;
		WorldToScreen(fSelectedCellX + 0.5f, fSelectedCellY + 0.5f, cx, cy);
		cr = 0.3f * fScaleX;
		FillCircle(cx, cy, cr, PIXEL_SOLID, FG_RED);
		auto function = [](float x)
		{
			return sinf(x);
		};
		// Draw chart
		float fWorldPerScreenWidthPixel = (fWorldRight - fWorldLeft) / ScreenWidth();
		float fWorldPerScreenHeightPixel = (fWorldBottom - fWorldTop) / ScreenHeight();
		int px, py, opx = 0, opy = 0;
		WorldToScreen(fWorldLeft - fWorldPerScreenWidthPixel, -function((fWorldLeft - fWorldPerScreenWidthPixel) - 5.0f), opx, opy);
		for (float x = fWorldLeft; x < fWorldRight; x += fWorldPerScreenWidthPixel)
		{
			float y = -function(x - 5.0f);
			WorldToScreen(x, y, px, py);
			DrawLine(opx, opy, px, py, PIXEL_SOLID, FG_GREEN);
			opx = px;
			opy = py;
		}
		DrawString(2, 2, L"Lines drawn: " + to_wstring(nLinesDrawn));
		return true;
	}

};
int main()
{
	OneLoneCoder_PanAndZoom demo;
	demo.ConstructConsole(160, 100, 8, 8);
	demo.Start();
	return 0;
}