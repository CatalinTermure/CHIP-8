#version 450

layout(binding = 0) uniform Display { bool pixels[64 * 32]; } display;
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform Size { vec2 size; } windowSize;

void main() {
  outColor = vec4(gl_FragCoord.x / windowSize.size.x, gl_FragCoord.y / windowSize.size.y, 0.0f, 1.0);
}