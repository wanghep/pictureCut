#pragma once
class PictureCutAlgorithm
{
public:
	PictureCutAlgorithm(void);
	~PictureCutAlgorithm(void);
	// variable 所有的轮廓的描述 vector


	//
	void colorPictureToGray();
	//检测出所有轮廓 // 
	//直接读取文件
	void pickOutAllContours();
	void showAllContours();
};

