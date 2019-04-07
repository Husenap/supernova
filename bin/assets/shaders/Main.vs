#include "Main.si"

PixelInput main(VertexInput input)
{
	PixelInput output;

	output.myPosition = input.myPosition;
	output.myUV = input.myUV;

	return output;
}
