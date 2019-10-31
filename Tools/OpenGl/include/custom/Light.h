#ifndef LIGHT_H
#define LIGHT

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light {
    public:
        /* ÊôÐÔ */  
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;        

        /* º¯Êý */
        Light() {};
        Light(std::vector<float> &data) {
            unsigned int index = 0;
            this->ambient = glm::vec3(data[index], data[index + 1], data[index + 2]);
            index += 3;
            this->diffuse = glm::vec3(data[index], data[index + 1], data[index + 2]);
            index += 3;
            this->specular = glm::vec3(data[index], data[index + 1], data[index + 2]);
        }
        Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
            setLightData(ambient, diffuse, specular);
        }
        void setLightData(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
            this->ambient = ambient;
            this->diffuse = diffuse;
            this->specular = specular;
        }  
};

class DirLight : public Light {
    using Light::Light;
    public:
        glm::vec3 direction = glm::vec3(0.0, 0.0, 0.0);
        DirLight() {};

        DirLight(std::vector<float> &data) : Light(data) {
            unsigned int index = 9;
            this->direction = glm::vec3(data[index], data[index + 1], data[index + 2]);               
        }

        DirLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction) : Light(ambient, diffuse, specular) {
            setDirDate(direction);
        }

        void setDirDate(glm::vec3 direction) {
            this->direction = direction;
        }
};

class PointLight : public Light {
    using Light::Light;

    public:
        glm::vec3 position;
        float constant;
        float linear;
        float quadratic;
        PointLight() {};
        PointLight(std::vector<float> &data) :Light(data){
            unsigned int index = 9;
            this->position = glm::vec3(data[index], data[index + 1], data[index + 2]);
            index += 3;
            this->constant = data[index];
            this->linear = data[index + 1];
            this->quadratic = data[index + 2];
        }
        PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, float constant, float linear, float quadratic) : Light(ambient, diffuse, specular) {
            setPointData(position, constant, linear, quadratic);
        }
        void setPointData(glm::vec3 position, float constant, float linear, float quadratic) {
            this->position = position;
            this->constant = constant;
            this->linear = linear;
            this->quadratic = quadratic;
        }
};

class SpotLight : public PointLight {
    using PointLight::PointLight;
    public:
        glm::vec3 direction;
        float cutOff;
        float outCutOff;
        SpotLight() {};
        SpotLight(std::vector<float> &data) : PointLight(data){
            unsigned int index = 15;
            this->direction = glm::vec3(data[index], data[index + 1], data[index + 2]);
            index += 3;
            this->cutOff = data[index];
            this->outCutOff = data[index + 1];
        }
        SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 direction, float cutOff, float outCutOff) :PointLight(ambient, diffuse, specular, position, constant, linear, quadratic) {
            setSpotData(direction, cutOff, outCutOff);
        }
        void setSpotData(glm::vec3 direction, float cutOff, float outCutOff) {
            this->direction = direction;
            this->cutOff = cutOff;
            this->outCutOff = outCutOff;
        }
};

#endif // !LIGHT_H
