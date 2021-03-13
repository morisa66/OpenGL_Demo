#ifndef __M_SHADER_H__
#define __M_SHADER_H__

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "../_morisa.h"
#include "../utils/m_log.h"

MORISA_NAMESPACE_BEGIN


class m_shader
{
public:
    m_shader(const char* v_path, const char* f_path)
    {
        using std::string;
        using std::ifstream;
        using std::stringstream;
        ifstream v_f, f_f;
        stringstream v_ss, f_ss;
        try
        {
            v_f.open(v_path);
            f_f.open(f_path);

            v_ss << v_f.rdbuf();
            f_ss << f_f.rdbuf();

            v_f.close();
            f_f.close();
        }
        catch (std::ifstream::failure&)
        {
#ifdef USE_LOG
            m_log("FILE_NOT_SUCCESFULLY_READ.");
#endif 
        }
        string s1 = v_ss.str();
        string s2 = f_ss.str();
        const char* v_code = s1.c_str();
        const char* f_code = s2.c_str();

        unsigned vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &v_code, NULL);
        glCompileShader(vertex);
        check_and_get_error_info(vertex);

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &f_code, NULL);
        glCompileShader(fragment);
        check_and_get_error_info(fragment);

        _ID = glCreateProgram();
        glAttachShader(_ID, vertex);
        glAttachShader(_ID, fragment);
        glLinkProgram(_ID);
        check_and_get_error_info(_ID, false);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    ~m_shader() { glDeleteProgram(_ID);  }

    void active()
    {
        glUseProgram(_ID);
    }

    unsigned get() const noexcept { return _ID; }

    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(_ID, name.c_str()), (int)value);
    }

    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(_ID, name.c_str()), value);
    }

    void setVec3(const std::string& name, glm::vec3 value) const
    {
        glUniform3f(glGetUniformLocation(_ID, name.c_str()), value.x, value.y, value.z);
    }

    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(_ID, name.c_str()), value);
    }

    void setMat4(const std::string& name, glm::mat4 value) {
        glUniformMatrix4fv(glGetUniformLocation(_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

private:
    unsigned _ID;

    void check_and_get_error_info(unsigned shader,  bool is_compile = true)
    {
        int success;
        char info[1024];
        if (is_compile)
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
#ifdef USE_LOG
                glGetShaderInfoLog(shader, 1024, NULL, info);
                m_log(info);
#endif          
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
#ifdef USE_LOG
                glGetProgramInfoLog(shader, 1024, NULL, info);
                m_log(info);
#endif      
            }
        }
    }
};

MORISA_NAMESPACE_END

#endif
