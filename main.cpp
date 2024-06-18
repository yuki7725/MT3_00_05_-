#include <Novice.h>
#include <cmath>

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x, y, z;
};

//3次元アフィン変換行列
//Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
Matrix4x4 MakeAffineMatrix(Matrix4x4 scaleMatrix, Matrix4x4 rotateXYZMatrix, Matrix4x4 translate);

//拡大縮小
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

//平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

//x軸回転行列
Matrix4x4 MakeRotateXMatrix(float rotate);

//y軸回転行列
Matrix4x4 MakeRotateYMatrix(float rotate);

//z軸回転行列
Matrix4x4 MakeRotateZMatrix(float rotate);


Matrix4x4 Multiply(const Matrix4x4 m1, const Matrix4x4 m2);


static const int kRowHeight = 20;
static const int kColumnWidth = 60;

static void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "%6.02f", matrix.m[row][column], label);
		}
	}
}


const char kWindowTitle[] = "GC2B_ナカノ_ユキ_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 scale(1.2f, 0.79f, -2.1f);
	Vector3 rotate(0.4f, 1.43f, -0.8f);
	Vector3 translate(2.7f, -4.15f, 1.57f);

	

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		
		//平行移動
		Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
		//拡大縮小
		Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

		//X軸回転
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
		//Y軸回転
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
		//Z軸回転
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
		//回転を合わせたもの
		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

		//ワールド変換
		Matrix4x4 worldMatrix = MakeAffineMatrix(scaleMatrix, rotateXYZMatrix, translateMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, worldMatrix, "worldMatrix");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}




//Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
//{
//	Matrix4x4 result = {};
//
//	result.m[0][0]=
//
//	return result;
//}

Matrix4x4 MakeAffineMatrix(Matrix4x4 scaleMatrix, Matrix4x4 rotateXYZMatrix, Matrix4x4 translate)
{
	Matrix4x4 result = {};

	result.m[0][0] = scaleMatrix.m[0][0] * rotateXYZMatrix.m[0][0];
	result.m[0][1] = scaleMatrix.m[0][0] * rotateXYZMatrix.m[0][1];
	result.m[0][2] = scaleMatrix.m[0][0] * rotateXYZMatrix.m[0][2];
	result.m[0][3] = 0;

	result.m[1][0] = scaleMatrix.m[1][1] * rotateXYZMatrix.m[1][0];
	result.m[1][1] = scaleMatrix.m[1][1] * rotateXYZMatrix.m[1][1];
	result.m[1][2] = scaleMatrix.m[1][1] * rotateXYZMatrix.m[1][2];
	result.m[1][3] = 0;

	result.m[2][0] = scaleMatrix.m[2][2] * rotateXYZMatrix.m[2][0];
	result.m[2][1] = scaleMatrix.m[2][2] * rotateXYZMatrix.m[2][1];
	result.m[2][2] = scaleMatrix.m[2][2] * rotateXYZMatrix.m[2][2];
	result.m[2][3] = 0;

	result.m[3][0] = translate.m[3][0];
	result.m[3][1] = translate.m[3][1];
	result.m[3][2] = translate.m[3][2];
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 result = {};

	result.m[0][0] = scale.x;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = scale.z;
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 result = {};

	result.m[0][0] = 1;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = 1;
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;
	result.m[2][3] = 0;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian)
{
	Matrix4x4 result = {};

	result.m[0][0] = 1;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = -std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian)
{
	Matrix4x4 result = {};

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = 0;
	result.m[0][2] = -std::sin(radian);
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = 1;
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = std::sin(radian);
	result.m[2][1] = 0;
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian)
{
	Matrix4x4 result = {};

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = -std::sin(radian);
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 Multiply(const Matrix4x4 m1, const Matrix4x4 m2)
{
	Matrix4x4 result = {};

	for (int m = 0; m < 4; m++) {
		for (int n = 0; n < 4; n++) {
			result.m[m][n] = 0;
			for (int i = 0; i < 4; i++) {
				result.m[m][n] += m1.m[m][i] * m2.m[i][n];
			}
		}
	}

	return result;
}
