
#pragma once

__interface IUINotifier
{
    // �ػ�UI(�������Ƿ������ػ�)
    void RedrawUI(BOOL bRedrawRightNow = FALSE);
    // �ػ�ͼ������(�������Ƿ������ػ�)
    void RedrawImg(BOOL bRedrawRightNow = FALSE);
    // ���ô��ڱ���
    void SetWindowTitle(LPCTSTR lpTitle);
    // �������ű���
    void SetZoomRate(LPCTSTR lpZoomRate);
    // �������ű���
    void SetZoomRate(float fRate);
    // ����ѡ��ľ�������
    void SetRectangle(const CPoint& ptLeftTop, const CPoint& ptRightBottom);

};