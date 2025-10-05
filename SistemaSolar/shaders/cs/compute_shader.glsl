#version 430

// Binding 0: must match your glBindImageTexture call
// Format: R32F for single-channel float buffer
layout(r32f, binding = 0) coherent uniform imageBuffer data;

layout(local_size_x = 4) in;

void main() {
    int idx = int(gl_GlobalInvocationID.x);
    // Load the current value
    float val = imageLoad(data, idx).x;
    // Add 1 and store back
    imageStore(data, idx, vec4(val + 1.0, 0.0, 0.0, 0.0));
}
