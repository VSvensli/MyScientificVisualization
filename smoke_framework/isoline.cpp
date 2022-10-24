#include "isoline.h"

#include <QDebug>

Isoline::Isoline(std::vector<float> const &values,
                 size_t const DIM,
                 float const isolineRho,
                 float const cellSideLength,
                 InterpolationMethod const interpolationMethod,
                 AmbiguousCaseDecider const ambiguousCaseDecider)
:
    m_values(values),
    m_DIM(DIM),
    m_isolineRho(isolineRho),
    m_cellSideLength(cellSideLength),
    m_ambiguousCaseDecider(ambiguousCaseDecider)

{
    //Code here
    for(size_t i = 0; i < m_DIM; i++){
        for(size_t j = 0; j < m_DIM; j++){

            std::vector<float> x(4);
            std::vector<float> y(4);
            std::vector<float> f(4);
            std::vector<bool>  s(4);

            for(size_t k = 0; k < 4; k++){
                x[k] = j;
                y[k] = i;

                if(k % 2 != 0){
                    x[k] = x[k] + 1;
                }
                if(k > 1){
                    y[k] = y[k] + 1;
                }

                if(x[k] == m_DIM){
                    x[k] = x[k]-1;
                }
                if(y[k] == m_DIM){
                    y[k] = y[k]-1;
                }

                int node_idx = int(x[k]) + int(y[k]*m_DIM);
                f[k] = m_values[node_idx];              // value at node k
                x[k] = m_cellSideLength * (1+x[k]);     // x coord at node k
                y[k] = m_cellSideLength * (1+y[k]);     // y coord at node k
                s[k] = f[k] > m_isolineRho;             // is node k inside(true) or outside(false) m_isolineRho
            }

            // Check if all nodes of cell is inside or outside the isoline; if so, skip
            if(std::all_of(s.begin(), s.end(), [](bool v) { return v; }) || std::all_of(s.begin(), s.end(), [](bool v) { return !v; })){
                continue;
            }

            QVector2D point;
            // Iso line x-axis
            if(s[0] != s[1]){
                point.setX(((f[1]-m_isolineRho)*x[0] + (m_isolineRho-f[0])*x[1])/(f[1]-f[0]));
                point.setY(y[0]);
                m_vertices.insert(m_vertices.end(),point);
            }

            // Iso line y-axis
            if(s[0] != s[2]){
                point.setX(x[0]);
                point.setY(((f[2]-m_isolineRho)*y[0] + (m_isolineRho-f[0])*y[2])/(f[2]-f[0]));
                m_vertices.insert(m_vertices.end(),point);
            }

            if(s[2] != s[3]){
                point.setX(((f[3]-m_isolineRho)*x[2] + (m_isolineRho-f[2])*x[3])/(f[3]-f[2]));
                point.setY(y[2]);
                m_vertices.insert(m_vertices.end(),point);
            }

            if(s[1] != s[3]){
                point.setX(x[1]);
                point.setY(((f[3]-m_isolineRho)*y[1] + (m_isolineRho-f[1])*y[3])/(f[3]-f[1]));
                m_vertices.insert(m_vertices.end(),point);
            }
            // If ambiguis case points will be added in the following order
            // p_01 p_02 p_23 p_13
            // If f[0] > m_isolineRho (positive) =! decition point THEN keep order, ELSE
            // New order = p_01 p_13 p_02 p_23 --> index change 0=0 1=2 2=3 3=1


            //Ambiguos if:
            if (s[0] != s[1] && s[0] != s[2] && s[0] == s[3]){
                float Ax = (f[0]-f[2])/(f[0]+f[3]-f[2]-f[1]);
                Ax = x[0] + m_cellSideLength* Ax;
                // If s[0] and mid_point are dissimilar, keep order.
                if(x[0] > Ax){
                    continue;
                }
                // If not, rearrange the last 3 indicies of m_vertices
                int last_idx = m_vertices.size()-1;
                QVector2D cache;
                cache = m_vertices[last_idx-2];
                m_vertices[last_idx-2] = m_vertices[last_idx-1];
                m_vertices[last_idx-1] = m_vertices[last_idx-0];
                m_vertices[last_idx-0] = cache;

            }
        }
    }
}

std::vector<QVector2D> Isoline::vertices() const
{
    return m_vertices;
}
