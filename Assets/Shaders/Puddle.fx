// Description: terrain fx file that blends textures
// the local space of vertices into NDC space

cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
    matrix world;
    float3 viewPosition;
}

cbuffer LightBuffer : register(b1)
{
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
    float3 lightDirection;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float4 materialEmissive;
    float materialPower;
}

cbuffer SettingsBuffer : register(b3)
{
    float blendHeight;
    float blendThickness;
    float puddleShininess;
    float puddleIntensity;
}

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D noiseMap : register(t2);

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 worldNormal : NORMAL;
    float3 worldTangent : Tangent;
    float2 texCoord : TEXCOORD;
    float3 dirToLight : TEXCOORD1;
    float3 dirToView : TEXCOORD2;
    float4 worldPosition : TEXCOORD3;
};

VS_OUTPUT VS(VS_INPUT input)
{
    float3 localPosition = input.position;
    
    VS_OUTPUT output;
    output.position = mul(float4(localPosition, 1.0f), wvp);
    output.worldPosition = mul(float4(localPosition, 1.0f), world);
    output.worldNormal = mul(input.normal, (float3x3) world);
    output.worldTangent = mul(input.tangent, (float3x3) world);
    output.texCoord = input.texCoord;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - (mul(float4(input.position, 1.0f), world).xyz));
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);

    // ambient color
    float4 ambient = lightAmbient * materialAmbient;
 
    // diffuse color
    float d = saturate(dot(light, n));
    float4 diffuse = d * lightDiffuse * materialDiffuse;

    // specular color
    float3 r = reflect(-light, n);
    float base = saturate(dot(r, view));
    float s = (puddleShininess > 0.0f) ? pow(base, materialPower * puddleShininess) : 0.0f;
    float4 specular = s * lightSpecular * materialSpecular;
 
    // emissive
    float4 emissive = materialEmissive;
    
    // texture sampling
    float4 baseColor = diffuseMap.Sample(textureSampler, input.texCoord);
    float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
    // No repititons
    float noiseValue = noiseMap.Sample(textureSampler, input.texCoord).r;
    // Repititions
    //float noiseValue = noiseMap.Sample(textureSampler, input.texCoord * 5.0f).r;
 
    // Height-based blending
    float blendStart = blendHeight - (blendThickness * 0.5f);
    float blendEnd = blendHeight + (blendThickness * 0.5f);
    float puddleFactor = saturate((blendHeight - input.worldPosition.y) / blendThickness);
    puddleFactor *= noiseValue * puddleIntensity;

    // Wet vs. Dry diffuse blending
    float4 wetDiffuseColor = lerp(baseColor, float4(0.2, 0.2, 0.25, 0.5), puddleFactor); // Wet areas darker/bluer
    diffuse *= lerp(1.0f, 0.7f, puddleFactor); // Reduce diffuse intensity in wet areas

    // Normals for Wet Effect
    float3 unpackedNormal = normalize(normalColor.rgb * 2.0f - 1.0f); // Unpack normal map
    unpackedNormal = lerp(unpackedNormal, float3(0.0, 0.0, 1.0), puddleFactor); // Flatten normals in wet areas
    n = normalize(n + unpackedNormal * puddleFactor); // Combine with world normal

    // Specular Highlights for Wet Areas
    float wetSpecularFactor = (puddleShininess > 0.0f) ? pow(saturate(dot(reflect(-light, n), view)), materialPower * puddleShininess) : 0.0f;
    specular += puddleFactor * wetSpecularFactor * lightSpecular;

    float4 finalColor = (emissive + ambient + diffuse) * wetDiffuseColor + specular;
    return finalColor;
}