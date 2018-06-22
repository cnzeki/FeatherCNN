//Tencent is pleased to support the open source community by making FeatherCNN available.

//Copyright (C) 2018 THL A29 Limited, a Tencent company. All rights reserved.

//Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
//in compliance with the License. You may obtain a copy of the License at
//
//https://opensource.org/licenses/BSD-3-Clause
//
//Unless required by applicable law or agreed to in writing, software distributed
//under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//CONDITIONS OF ANY KIND, either express or implied. See the License for the
//specific language governing permissions and limitations under the License.

#pragma once

#include "../feather_simple_generated.h"
#include "../layer.h"

#ifdef FEATHER_AVX
#include "avx/sgemv.h"
#else
#include "arm/sgemv.h"
#endif

#include <assert.h>
#include <stdio.h>

namespace feather
{
class InnerProductLayer : public Layer
{
    public:
        InnerProductLayer(const LayerParameter *layer_param, const RuntimeParameter<float>* rt_param)
            : Layer(layer_param, rt_param), fuse_relu(false)
        {
            //From proto
            const InnerProductParameter *inner_product_param = layer_param->inner_product_param();
            bias_term = inner_product_param->bias_term();

            assert(_weight_blobs.size() > 0);
            kernel_data = this->_weight_blobs[0]->data();
            output_channels = this->_weight_blobs[0]->num();
            input_channels = this->_weight_blobs[0]->channels();
	    printf("blob shape %d %d\n", output_channels, input_channels);
            if (bias_term)
            {
                assert(this->_weight_blobs.size() == 2);
                bias_data = this->_weight_blobs[1]->data();
            }
	    _fusible = false;
        }

        int Forward()
        {
            const float *input = _bottom_blobs[_bottom[0]]->data();
            float *output = _top_blobs[_top[0]]->data();

#ifdef FEATHER_ARM
            if (output_size % 8 == 0 && input_size % 8 == 0)
                fully_connected_transpose_inference_neon8((int)input_size, (int)output_size, input, kernel_data, output, num_threads);
            else
                fully_connected_inference_direct((int)input_size, (int)output_size, input, kernel_data, output, num_threads);
            if (bias_term)
                for (int i = 0; i < output_size; i++)
                    output[i] += bias_data[i];
#endif
#ifdef FEATHER_AVX
	   packed_sgemv_kernel(output_size, input_size, input, kernel_data, output, bias_data, num_threads);
#endif

            return 0;
        }

        int Init()
        {
#ifdef FEATHER_ARM
            float* buffer = NULL;
            MEMPOOL_CHECK_RETURN(private_mempool.Alloc(&buffer, sizeof(float) * input_size * 8));
            if (input_size % 8 == 0 && output_size % 8 == 0)
            {
                for (int i = 0; i < output_size / 8; i++)
                    matrixTranspose(kernel_data + i * 8 * input_size, 8, input_size, buffer);
            }
            else
            {
                //Naive implementation doesn't require preprocess
            }
            MEMPOOL_CHECK_RETURN(private_mempool.Free(&buffer));
#endif
#ifdef FEATHER_AVX
	    float* packed_buffer;
	    MEMPOOL_CHECK_RETURN(private_mempool.Alloc(&packed_buffer, sizeof(float) * 16 * input_size));
	    packed_sgemv_transposed_init<16>(output_size, input_size, kernel_data, packed_buffer);
	    MEMPOOL_CHECK_RETURN(private_mempool.Free(&packed_buffer));
	    if(bias_term && fuse_relu)
	    {
		packed_sgemv_kernel = packed_sgemv<true, true>;	
	    } 
	    else if(bias_term && !fuse_relu)
	    {
		packed_sgemv_kernel = packed_sgemv<true, false>;
	    } 
	    else if(!bias_term && fuse_relu)
	    {
		packed_sgemv_kernel = packed_sgemv<false, true>;
	    } 
	    else 
	    {
		packed_sgemv_kernel = packed_sgemv<false, false>;
	    }
	    
#endif
            return 0;
        }

	int Fuse(Layer *next_layer)
	{
		if (next_layer->type().compare("ReLU") == 0 && !fuse_relu)
		{
			printf("FC %s fuse ReLU layer %s\n", this->name().c_str(), next_layer->name().c_str());
			fuse_relu = true;
			return 1;
		}
		else
			return 0;
	}

        int GenerateTopBlobs()
        {
            //InnerProduct layer has and only has one bottom blob.
            const Blob<float> *bottom_blob = _bottom_blobs[_bottom[0]];
            input_width = bottom_blob->width();
            input_height = bottom_blob->height();
            input_channels = bottom_blob->channels();
            input_size = bottom_blob->data_size();
            _top_blobs[_top[0]] = new Blob<float>(1, output_channels, 1, 1);
            _top_blobs[_top[0]]->Alloc();
            output_size = _top_blobs[_top[0]]->data_size();
            return 0;
        }

    protected:
        //Legacy
        size_t input_channels;
        size_t input_width;
        size_t input_height;

        size_t output_channels;

        size_t input_size;
        size_t output_size;
        bool bias_term;
	bool fuse_relu;

        float *kernel_data;
        float *bias_data;

	void (*packed_sgemv_kernel)(const int N, const int K, const float* A, const float* B, float* C, const float* bias_data, const int num_threads);
};
};
