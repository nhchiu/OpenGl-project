#version 410

#define MAX_POINT_LIGHTS 3
#define MAX_SPOT_LIGHTS 3

in vec4 vertex_color;
in vec2 tex_coord;
in vec3 normal;
in vec3 frag_pos;

out vec4 color;

struct Light {
    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
};

struct DirectionalLight {
    Light base;
    vec3 direction;
};

struct PointLight {
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight {
    PointLight base;
    vec3 direction;
    float edge;
};

struct Material {
    float specular_intensity;
    float shininess;
};

uniform int point_light_count;
uniform int spot_light_count;

uniform DirectionalLight directional_light;
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS];

uniform sampler2D the_texture;
uniform Material material;

uniform vec3 eye_position;

vec3 CalcLightDirection(Light light, vec3 direction) {
    vec3 ambient_color = light.color * light.ambient_intensity;

    float diffuse_factor = dot(-normalize(normal), normalize(direction));
    vec3 diffuse_color = (diffuse_factor > 0.0f)? (light.color * diffuse_factor * light.diffuse_intensity) : vec3(0.0f, 0.0f, 0.0f);

    vec3 specular_color = vec3(0.0f, 0.0f, 0.0f);
    if (diffuse_factor > 0.0f) {
        vec3 frag_to_eye = normalize(eye_position - frag_pos);
        vec3 reflected_light = normalize(reflect(direction, -normalize(normal)));

        float specular_factor = max(dot(frag_to_eye, reflected_light), 0.0f);

        if (specular_factor > 0.0f) {
            specular_factor = pow(specular_factor, material.shininess);
            specular_color = light.color * specular_factor * material.specular_intensity;
        }
    }

    return (ambient_color + diffuse_color + specular_color);
}

vec3 CalcDirectionalLight() {
    return CalcLightDirection(directional_light.base, directional_light.direction);
}

vec3 CalcPointLight(PointLight p_light) {
    vec3 direction = (frag_pos - p_light.position);
    float dis = length(direction);

    float attenuation = 1 / (p_light.exponent * dis * dis +
                             p_light.linear * dis +
                             p_light.constant);

    return (CalcLightDirection(p_light.base, normalize(direction)) * attenuation);
}

vec3 CalcPointLights() {
    vec3 total_color = vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < point_light_count; i++) {
        total_color += CalcPointLight(point_lights[i]);
    }
    return total_color;
}

vec3 CalcSpotLight(SpotLight s_light) {
    vec3 ray = normalize(frag_pos - s_light.base.position);
    float sl_factor = dot(ray, s_light.direction);

    if (sl_factor > s_light.edge) {
        vec3 color = CalcPointLight(s_light.base);
        return color * (1.0f - ((1.0f - sl_factor) / (1.0f - s_light.edge)));
    }
    else {
        return vec3(0.0f, 0.0f, 0.0f);
    }
}

vec3 CalcSpotLights() {
    vec3 total_color = vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < spot_light_count; i++) {
        total_color += CalcSpotLight(spot_lights[i]);
    }
    return total_color;
}

void main(){
    vec3 final_color = CalcDirectionalLight() + CalcPointLights() + CalcSpotLights();

    color = texture(the_texture, tex_coord) * vec4(final_color, 1.0f);
}
