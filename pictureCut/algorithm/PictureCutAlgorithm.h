#pragma once
class PictureCutAlgorithm
{
public:
	PictureCutAlgorithm(void);
	~PictureCutAlgorithm(void);
	// variable ���е����������� vector


	//
	void colorPictureToGray();
	//������������ // 
	//ֱ�Ӷ�ȡ�ļ�
	void pickOutAllContours();
	void showAllContours();
};

