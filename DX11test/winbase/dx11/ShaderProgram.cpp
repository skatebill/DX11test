#include "ShaderProgram.h"
HRESULT compileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
HRESULT ShaderProgram::loadShader(WCHAR* fileName)
{
	if(!m_layout){
		return S_FALSE;
	}
    ID3DBlob* pVSBlob = NULL;
	HRESULT hr = compileShaderFromFile( fileName, "VS", "vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
		return hr;
	
    }
	hr = m_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release();
		return hr;
	}

    // Create the input layout
	hr = m_pd3dDevice->CreateInputLayout( m_layout, m_numElemnts, pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), &m_pVertexLayout );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release();
		return hr;
	}
	pVSBlob->Release();
	
	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
    hr = compileShaderFromFile( fileName, "PS", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
		return hr;
    }

	// Create the pixel shader
	hr = m_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader );
	pPSBlob->Release();
	return S_OK;
}

void ShaderProgram::setConstantBuffer(ID3D11DeviceContext* pContext,int slot,void* buffer)
{	
	pContext->UpdateSubresource( m_bufferMap[slot], 0, NULL, buffer, 0, 0 );
	pContext->VSSetConstantBuffers(slot,1,&(m_bufferMap[slot]));
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT compileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();
    return S_OK;
}