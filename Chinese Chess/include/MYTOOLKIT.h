#ifndef MYTOOLKIT_H
#define MYTOOLKIT_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
#include<CAMERA.h>
#include<SHADER.h>
#include<MODEL.h>
#include<TEXTURE.h>
#include <vector>
class Toolkit
{
public:
	static Camera *myCamera;
	static vector<Shader *>myShader;
	static int modelShader;
	static GLFWwindow * window;
	static int scrWidth, scrHeight;

public:
	static void createCamera(int width, int height)
	{
		if (myCamera != NULL)delete myCamera;
		myCamera = new Camera(width, height);
	}
	static int createShader(const char * vertexShaderSource, const char * fragmentShaderSource,const char *gshader=NULL)
	{
		Shader * temp = new Shader(vertexShaderSource, fragmentShaderSource,gshader);
		myShader.push_back(temp);
		return myShader.size() - 1;
	}

	static int InitOpenGL(const char *name,const unsigned int SCR_WIDTH,const unsigned int SCR_HEIGHT)
	{
		scrWidth = SCR_WIDTH;
		scrHeight = SCR_HEIGHT;
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, name, NULL, NULL);
		if (window == NULL)
		{
			cout << "Failed to create window" << endl;
			glfwTerminate();
			return 0;
		}
		Toolkit::createCamera(SCR_WIDTH, SCR_HEIGHT);
		if (myCamera == NULL)
		{
			cout << "Initializing Camera failed!" << endl;
			return 0;
		}
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		Camera::registerCamera(window, myCamera);
		
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			cout << "Failed to initialize glad" << endl;
			return 0;
		}
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_FRAMEBUFFER_SRGB);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		return 1;
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	static void checkScrShotRequest()
	{
		static bool gPressed = false;
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		{
			if (gPressed == false)
			{
				gPressed = true;
				screenShot();
			}
		}
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE)gPressed = false;
	}
	static void screenShot()
	{
		static const int BMP_Header_Length = 54;
		FILE*    pDummyFile;
		FILE*    pWritingFile;
		GLubyte* pPixelData;
		GLubyte  BMP_Header[BMP_Header_Length];
		GLint    i, j;
		GLint    PixelDataLength;

		// �����������ݵ�ʵ�ʳ���
		i = scrWidth * 3;   // �õ�ÿһ�е��������ݳ���
		while (i % 4 != 0)      // �������ݣ�ֱ��i�ǵı���
			++i;               // �������и�����㷨��
							   // �������׷��ֱ�ۣ����ٶ�û��̫��Ҫ��
		PixelDataLength = i * scrHeight;

		// �����ڴ�ʹ��ļ�
		pPixelData = (GLubyte*)malloc(PixelDataLength);
		if (pPixelData == 0)
			exit(0);
		fopen_s(&pDummyFile, "dummy.bmp", "rb");
		if (pDummyFile == 0)
			exit(0);
		fopen_s(&pWritingFile, "grab.bmp", "wb");
		if (pWritingFile == 0)
			exit(0);

		// ��ȡ����
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glReadPixels(0, 0, scrWidth, scrHeight,
			GL_BGR, GL_UNSIGNED_BYTE, pPixelData);

		// ��dummy.bmp���ļ�ͷ����Ϊ���ļ����ļ�ͷ
		fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
		fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
		
		fseek(pWritingFile, 0x0012, SEEK_SET);
		i = scrWidth;
		j = scrHeight;
	
		fwrite(&i, sizeof(i), 1, pWritingFile);
		fwrite(&j, sizeof(j), 1, pWritingFile);

	
		// д����������
		fseek(pWritingFile, 0, SEEK_END);
		fwrite(pPixelData, PixelDataLength, 1, pWritingFile);
		
		// �ͷ��ڴ�͹ر��ļ�
		fclose(pDummyFile);
		fclose(pWritingFile);
		free(pPixelData);
		cout << "successfully catch the new frame!" << endl;
	}
	
};

vector<Shader *> Toolkit::myShader;
Camera *Toolkit::myCamera=NULL;
GLFWwindow *Toolkit::window = NULL;
int Toolkit::scrWidth;
int Toolkit::scrHeight;
int Toolkit::modelShader;

#endif // !MYTOOLKIT_H
