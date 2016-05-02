
#pragma once

__interface IUINotifier
{
    void RedrawUI();
    void RedrawImg();
    void SetWindowTitle(LPCTSTR lpTitle);
    void SetZoomRate(LPCTSTR lpZoomRate);
    void SetZoomRate(float fRate);

};