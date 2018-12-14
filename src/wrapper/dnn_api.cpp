#include "dnn_api.hpp"
#include <algorithm>
#include <string>
#include <vector>

#include "androidlog.h"
#include "net.h"
using std::string;
using std::vector;

using namespace feather;

// Blob4F
int Blob4F::num()
{
	return dims[0];
}
int Blob4F::channels()
{
	return dims[1];
}
int Blob4F::height()
{
	return dims[2];
}

int Blob4F::count()
{
	return dims[0] * dims[1] * dims[2] * dims[3];
}

int Blob4F::width()
{
	return dims[3];
}

Blob4F::Blob4F()
{
	ownData = false;
	reset();
}

void Blob4F::reset()
{
	dims[0] = dims[1] = dims[2] = dims[3] = 0;
	data = NULL;
}

void Blob4F::create(int n,int ch,int h,int w)
{
	dims[0] = n;
	dims[1] = ch;
	dims[2] = h;
	dims[3] = w;
	data = new float[n * ch * w * h];
	ownData = true;
}

void Blob4F::createNoCopy(int n, int ch, int h, int w, float* sharedData)
{
	dims[0] = n;
	dims[1] = ch;
	dims[2] = h;
	dims[3] = w;
	data = sharedData;
	ownData = false;
}

void Blob4F::destroy()
{
	if(ownData && data)
	{
		delete data;
		data = NULL;
	}
	reset();
}

int	caffe_set_mode(int isGpu, int gpuId)
{
	LOGE("set mode not support");
	return 0;
}

/* Prediction APIs */
// Load a pretrained net
NetHandle 	caffe_net_load(const char* prototxt, const char* weights, int isPhaseTest )
{
	Net* net = new Net(1);
	if (!net)
	{
		return NULL;
	}
	net->InitFromPath(prototxt);
	return (NetHandle)net;
}

// Load a net structure
NetHandle caffe_net_loadRaw(const char* prototxt, int isPhaseTest )
{
	Net* net = new Net(1);
	if (!net)
	{
		return NULL;
	}
	net->InitFromPath(prototxt);
	return (NetHandle)net;
}

void caffe_net_release(NetHandle net)
{
	Net* pNet = (Net*)net;
	if (!pNet)
	{
		return ;
	}

	delete pNet;
}

// Set blob data for given blob name
bool caffe_net_setBlob(NetHandle net, const char* blobName, Blob4F blob)
{
	Net* pNet = (Net*)net;
	if (!pNet)
	{
		return false;
	}
	std::string name = std::string(blobName);
	int ret = pNet->FeedBlob(blob.data, blob.dims, blobName);
	return ret >= 0;
}

// Get blob data for given blob name
Blob4F caffe_net_getBlob(NetHandle net, const char* blobName)
{
	Blob4F blob;
	std::string name = std::string(blobName);
	int dims[4] = { 0 };
	Net* pNet = (Net*)net;
	if (!pNet)
	{
		return blob;
	}
	
	if (pNet->GetBlobDims(blob.dims, name) < 0)
	{
		return blob;
	}
	blob.create(dims[0], dims[1], dims[2], dims[3]);
	pNet->ExtractBlob(blob.data, name);
	return blob;
}

// Run forward 
void caffe_net_forward(NetHandle net)
{
	Net* pNet = (Net*)net;
	if (!pNet)
	{
		return ;
	}
	pNet->Forward();
}
	
void caffe_net_forwardFromTo(NetHandle net, int from, int to)
{
	Net* pNet = (Net*)net;
	if (!pNet)
	{
		return;
	}
	pNet->ForwardFromTo(from,to);
}

const std::vector<std::string> caffe_net_getLayerNames(NetHandle net)
{
	std::vector<std::string> names;
	Net* pNet = (Net*)net;
	if (!pNet)
	{
		return names;
	}
	return pNet->GetLayerNames();
}

int caffe_net_getLayerIndex(NetHandle net, const char* layerName)
{
	Net* pNet = (Net*)net;
	if (!pNet)
	{
		return -1;
	}

	std::string name(layerName);
	return pNet->GetLayerIndex(name);
}

int caffe_net_getInputBlobSize(NetHandle net)
{
	Net* pNet = (Net*)net;
	if (!pNet)
	{
		return 0;
	}
	return pNet->GetInputBlobSize();
}

const std::string caffe_net_getInputBlobName(NetHandle net, int index)
{
	Net* pNet = (Net*)net;
	return pNet->GetInputBlobName(index);
}

//////////////////////////////////////////////////////////////////////////////////
// Compatible with old apis
CAFFE_HANDLE caffe_create_handle(const char* model_path, const char* weights_path)
{
	return caffe_net_load(model_path,weights_path);
}

void caffe_release_handle(CAFFE_HANDLE handle)
{
	caffe_net_release(handle);
}


/****************************************************************************************\
*                                  Matrix type (CvMat)                                   *
\****************************************************************************************/

#define CV_CN_MAX     512
#define CV_CN_SHIFT   3
#define CV_DEPTH_MAX  (1 << CV_CN_SHIFT)

#define CV_8U   0
#define CV_8S   1
#define CV_16U  2
#define CV_16S  3
#define CV_32S  4
#define CV_32F  5
#define CV_64F  6
#define CV_USRTYPE1 7

#define CV_MAT_DEPTH_MASK       (CV_DEPTH_MAX - 1)
#define CV_MAT_DEPTH(flags)     ((flags) & CV_MAT_DEPTH_MASK)

#define CV_MAKETYPE(depth,cn) (CV_MAT_DEPTH(depth) + (((cn)-1) << CV_CN_SHIFT))
#define CV_MAKE_TYPE CV_MAKETYPE

#define CV_8UC1 CV_MAKETYPE(CV_8U,1)
#define CV_8UC2 CV_MAKETYPE(CV_8U,2)
#define CV_8UC3 CV_MAKETYPE(CV_8U,3)
#define CV_8UC4 CV_MAKETYPE(CV_8U,4)
#define CV_8UC(n) CV_MAKETYPE(CV_8U,(n))

#define CV_8SC1 CV_MAKETYPE(CV_8S,1)
#define CV_8SC2 CV_MAKETYPE(CV_8S,2)
#define CV_8SC3 CV_MAKETYPE(CV_8S,3)
#define CV_8SC4 CV_MAKETYPE(CV_8S,4)
#define CV_8SC(n) CV_MAKETYPE(CV_8S,(n))

#define CV_16UC1 CV_MAKETYPE(CV_16U,1)
#define CV_16UC2 CV_MAKETYPE(CV_16U,2)
#define CV_16UC3 CV_MAKETYPE(CV_16U,3)
#define CV_16UC4 CV_MAKETYPE(CV_16U,4)
#define CV_16UC(n) CV_MAKETYPE(CV_16U,(n))

#define CV_16SC1 CV_MAKETYPE(CV_16S,1)
#define CV_16SC2 CV_MAKETYPE(CV_16S,2)
#define CV_16SC3 CV_MAKETYPE(CV_16S,3)
#define CV_16SC4 CV_MAKETYPE(CV_16S,4)
#define CV_16SC(n) CV_MAKETYPE(CV_16S,(n))

#define CV_32SC1 CV_MAKETYPE(CV_32S,1)
#define CV_32SC2 CV_MAKETYPE(CV_32S,2)
#define CV_32SC3 CV_MAKETYPE(CV_32S,3)
#define CV_32SC4 CV_MAKETYPE(CV_32S,4)
#define CV_32SC(n) CV_MAKETYPE(CV_32S,(n))

#define CV_32FC1 CV_MAKETYPE(CV_32F,1)
#define CV_32FC2 CV_MAKETYPE(CV_32F,2)
#define CV_32FC3 CV_MAKETYPE(CV_32F,3)
#define CV_32FC4 CV_MAKETYPE(CV_32F,4)
#define CV_32FC(n) CV_MAKETYPE(CV_32F,(n))

#define CV_64FC1 CV_MAKETYPE(CV_64F,1)
#define CV_64FC2 CV_MAKETYPE(CV_64F,2)
#define CV_64FC3 CV_MAKETYPE(CV_64F,3)
#define CV_64FC4 CV_MAKETYPE(CV_64F,4)
#define CV_64FC(n) CV_MAKETYPE(CV_64F,(n))

#define CV_AUTO_STEP  0x7fffffff
#define CV_WHOLE_ARR  cvSlice( 0, 0x3fffffff )

#define CV_MAT_CN_MASK          ((CV_CN_MAX - 1) << CV_CN_SHIFT)
#define CV_MAT_CN(flags)        ((((flags) & CV_MAT_CN_MASK) >> CV_CN_SHIFT) + 1)
#define CV_MAT_TYPE_MASK        (CV_DEPTH_MAX*CV_CN_MAX - 1)
#define CV_MAT_TYPE(flags)      ((flags) & CV_MAT_TYPE_MASK)
#define CV_MAT_CONT_FLAG_SHIFT  14
#define CV_MAT_CONT_FLAG        (1 << CV_MAT_CONT_FLAG_SHIFT)
#define CV_IS_MAT_CONT(flags)   ((flags) & CV_MAT_CONT_FLAG)
#define CV_IS_CONT_MAT          CV_IS_MAT_CONT
#define CV_SUBMAT_FLAG_SHIFT    15
#define CV_SUBMAT_FLAG          (1 << CV_SUBMAT_FLAG_SHIFT)
#define CV_IS_SUBMAT(flags)     ((flags) & CV_MAT_SUBMAT_FLAG)

#define CV_MAGIC_MASK       0xFFFF0000
#define CV_MAT_MAGIC_VAL    0x42420000
#define CV_TYPE_NAME_MAT    "opencv-matrix"

std::vector<float>  caffe_extract_feature(CAFFE_HANDLE handle, const char* blob_name,
	unsigned char* bgr, int type, int width, int height)
{
	std::vector<float> ret;
	int channels = CV_MAT_CN(type);
	int depth = CV_MAT_DEPTH(type);
	// only surport uchar and float input
	if (CV_8U != depth && CV_32F != depth)
	{
		return ret;
	}

	if (!handle)
	{
		return ret;
	}

	// fill blob
	Blob4F inputBlob;
	inputBlob.create(1, channels,height,width);
	switch (depth)
	{
	case CV_8U:
		ImageToTensor<unsigned char, float>(bgr, height,width, channels,inputBlob.data,inputBlob.dims);
		break;
	case CV_32F:
		ImageToTensor<float, float>((float*)bgr, height,width, channels, inputBlob.data, inputBlob.dims);
		break;
	}

	// get input name
	const std::string inputName = caffe_net_getInputBlobName(handle, 0);
	// feed to net
	caffe_net_setBlob(handle, inputName.c_str(), inputBlob);
	// run forward
	caffe_net_forward(handle);
	// extract result
	Blob4F featBlob = caffe_net_getBlob(handle, blob_name);
	Blob4FToVector<float>(featBlob, ret);
	featBlob.destroy();
	inputBlob.destroy();
	return ret;
}

