#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

void getZeroPaddingImage(Mat& src, Mat& dst) {
	//적절한 푸리에 변환 사이즈 찾기
	int m = getOptimalDFTSize(src.rows);
	int n = getOptimalDFTSize(src.cols);
	copyMakeBorder(src, dst, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));


}

//실수부와 허수부 갑으로부터 크기이와 각도를 계산하고 크기에 log취하여 돌려주는 함수
void getLogMag(Mat planes[], Mat& magI, Mat& angI) {
	//극 좌표계로 변환
	//x,y,크기,각도값,true<-360도 기준
	cartToPolar(planes[0], planes[1], magI, angI, true);
	//1보다 큰 값을 넣어서 로그를 취할때 문제가 없도록 함
	magI += Scalar::all(1);
	log(magI, magI);

	//스펙트럼 이미지가 원 이미지처럼 변화시켜주기 위해 잘라냄
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
}

void rearrange(Mat magI) {
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;
	Mat q0(magI, Rect(0, 0, cx, cy));
	Mat q1(magI, Rect(cx, 0, cx, cy));
	Mat q2(magI, Rect(0, cy, cx, cy));
	Mat q3(magI, Rect(cx, cy, cx, cy));
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
}

void myFilter(Mat& magI_org,Mat& magl_logo) {
	Mat highpass_filtedImg;
	Mat lowpass_filterdImg;
	Mat highPassFilter= Mat::ones(magI_org.rows, magI_org.cols, CV_32F);

	int cx = magI_org.cols / 2;
	int cy = magI_org.rows / 2;
	circle(highPassFilter, Point(cx, cy), 200, Scalar(0), -1);
	imshow("before magl_org", magI_org);

	highpass_filtedImg = magI_org.clone().mul(highPassFilter);
	imshow("beforHighpass filter", highpass_filtedImg);

	lowpass_filterdImg = magI_org - highpass_filtedImg;
	imshow("lowpass filter", lowpass_filterdImg);

	highpass_filtedImg = highpass_filtedImg.mul(magl_logo);
	imshow("afterHighpassfilter",highpass_filtedImg);
	
	magI_org = lowpass_filterdImg + highpass_filtedImg;
	
	imshow("after magl_org", magI_org);
}

int main(void){
	Mat I=imread("lena512.jpg", IMREAD_GRAYSCALE);

	Mat logo = imread("LOGO.png", IMREAD_GRAYSCALE);
	resize(logo, logo, Size(I.cols, I.rows));
	imshow("LOGO", logo);

	//제로패딩
	Mat padded1;
	Mat padded2;
	getZeroPaddingImage(I, padded1);
	getZeroPaddingImage(logo, padded2);

	//dft를 위한 2채널 매트릭스 생성
	//푸리에 변환을 하면 실수로 나와서 float
	Mat planes1[] = { Mat_<float>(padded1),Mat::zeros(padded1.size(),CV_32F) };
	Mat complexI1;
	merge(planes1, 2, complexI1);

	Mat planes2[] = { Mat_<float>(padded2),Mat::zeros(padded2.size(),CV_32F) };
	Mat complexI2;
	merge(planes2, 2, complexI2);

	//fft 수행하고 채널 분리 plane[0]:실수부, plane[1]:허수부
	dft(complexI1, complexI1);
	split(complexI1, planes1);

	dft(complexI2, complexI2);
	split(complexI2, planes2);

	//크기와 각도를 계산하는 식이  필요
	Mat magI_org, angI_org;
	Mat magI_logo, angI_logo;

	getLogMag(planes1, magI_org, angI_org);
	getLogMag(planes2, magI_logo, angI_logo);

	//역변환을 위한 최대 최소값 계산
	double min_mag_org, max_mag_org, min_ang_org, max_ang_org;
	minMaxLoc(magI_org, &min_mag_org, &max_mag_org);
	minMaxLoc(angI_org, &min_ang_org, &max_ang_org);

	double min_mag_logo, max_mag_logo, min_ang_logo, max_ang_logo;
	minMaxLoc(magI_logo, &min_mag_logo, &max_mag_logo);
	minMaxLoc(angI_logo, &min_ang_logo, &max_ang_logo);

	//정규화
	normalize(magI_org, magI_org, 0, 1, NORM_MINMAX);
	normalize(angI_org, angI_org, 0, 1, NORM_MINMAX);

	normalize(magI_logo, magI_logo, 0, 1, NORM_MINMAX);
	normalize(angI_logo, angI_logo, 0, 1, NORM_MINMAX);

	rearrange(magI_org);
	rearrange(angI_org);

	rearrange(magI_logo);
	rearrange(angI_logo);

	//필터링
	myFilter(magI_org,magI_logo);
	imshow("filtered mag", magI_org);
	//Inverse dft

	//위치 복귀
	rearrange(magI_org);
	rearrange(angI_org);

	//de-normalize
	magI_org = magI_org * (max_mag_org - min_mag_org) + min_mag_org;
	angI_org = angI_org * (max_ang_org - min_ang_org) + min_ang_org;

	//지수변환
	exp(magI_org, magI_org);
	magI_org -= 1;

	//직교좌표계로 변환하고 Inverse FFT 수행
	polarToCart(magI_org, angI_org, planes1[0], planes1[1], true);
	merge(planes1, 2, complexI1);

	//Inverse FFT
	dft(complexI1, complexI1, DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);
	split(complexI1, planes1);
	normalize(planes1[0], planes1[0], 0, 1, NORM_MINMAX);
	//imshow("정규화 이전 magI", magI);
	//imshow("정규화 이후 anngI", angI);
	imshow("IDFT", planes1[0]);

	waitKey(0);
	return 0;

}
