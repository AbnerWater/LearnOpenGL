#pragma once
#include <H5Cpp.h>
#include <string.h>
#include <vector>
#define PI 3.1415926535897932384626433832795028842
using namespace std;
using namespace H5;
struct Node {
	int64_t id;//节点ID
	int64_t cp;
	double coord[3];//节点坐标
	int64_t cd;
	int64_t ps;
	int64_t seid;
	int64_t domain;//域
};
struct TETRA_INPUT {
	int64_t eid;//单元ID
	int64_t pid;
	int64_t connection[10];//单元拓扑
	int64_t domain;//域
}; 
struct HEXA_INPUT {
	int64_t eid;//单元ID
	int64_t pid;
	int64_t connection[20];//单元拓扑
	int64_t domain;//域
};
struct DISPLACEMENT {
	int64_t id;//节点ID
	double x;//x位移
	double y;//y位移
	double z;//z位移
	double rx;//x旋转
	double ry;//y旋转
	double rz;//z旋转
	int64_t domain;//域
};
class H5Reader
{
public:
	const string NODE_PATH = "/NASTRAN/INPUT/NODE/GRID";
	const string ELEMENT_PATH = "/NASTRAN/INPUT/ELEMENT";
	const string DISPLACEMENT_PATH = "/NASTRAN/RESULT/NODAL/DISPLACEMENT";
	H5Reader(const char* path) {
		m_file = string(path);
		try
		{
			m_h5data = new H5File();
			m_h5data->openFile(m_file, H5F_ACC_RDONLY);
		}
		catch (H5::Exception error)
		{
			m_h5data = NULL;
		}
	}
	~H5Reader() {
		m_h5data->close();
	}
	bool Avilable() { return m_h5data != NULL; }
	vector<float> GetNodes() const {
		vector<float> ns;
		DataSet node_set = m_h5data->openDataSet(NODE_PATH);
		CompType node_t = node_set.getCompType();
		hsize_t length;
		node_set.getSpace().getSimpleExtentDims(&length);
		Node *nodes = new Node[length];
		node_set.read(nodes, node_t);
		for (int i = 0; i < length; i++) {
			auto d = nodes[i];
			switch (d.cp) {
			case 0:
				ns.push_back((float)(d.coord[0])*0.1);
				ns.push_back((float)(d.coord[1])*0.1);
				ns.push_back((float)(d.coord[2])*0.1);
				break;
			case 1:
				ns.push_back((float)(d.coord[0] * cos(d.coord[1] * PI / 180)));
				ns.push_back((float)(d.coord[0] * sin(d.coord[1] * PI / 180)));
				ns.push_back((float)(d.coord[2]));
				break;
			case 2:
				ns.push_back((float)(d.coord[0]));
				ns.push_back((float)(d.coord[1]));
				ns.push_back((float)(d.coord[2]));
				break;
			}
		}
		free(nodes);
		node_set.close();
		return ns;
	}
	vector<unsigned int> GetElement() const {
		vector<unsigned int> eles;
		Group element_set = m_h5data->openGroup(ELEMENT_PATH);
		int num = element_set.getNumObjs();
		for (int i = 0; i < num; i++)
		{
			string elementname = element_set.getObjnameByIdx(i);
			DataSet element = m_h5data->openDataSet(ELEMENT_PATH + "/" + elementname);
			CompType element_t = element.getCompType();
			hsize_t length;
			element.getSpace().getSimpleExtentDims(&length);
			if (elementname == "CTETRA"){
				TETRA_INPUT *elements = new TETRA_INPUT[length];
				element.read(elements, element_t);
				for (int i = 0; i < length; i++) {
					auto d = elements[i];
					eles.push_back(d.connection[0] - 1); eles.push_back(d.connection[1] - 1); eles.push_back(d.connection[2] - 1);
					eles.push_back(d.connection[0] - 1); eles.push_back(d.connection[1] - 1); eles.push_back(d.connection[3] - 1);
					eles.push_back(d.connection[1] - 1); eles.push_back(d.connection[2] - 1); eles.push_back(d.connection[3] - 1);
					eles.push_back(d.connection[0] - 1); eles.push_back(d.connection[2] - 1); eles.push_back(d.connection[3] - 1);
				}
				free(elements);
			}
			else if (elementname == "CHEXA") {
				HEXA_INPUT *elements = new HEXA_INPUT[length];
				element.read(elements, element_t);
				for (int i = 0; i < length; i++) {
					auto d = elements[i];
					eles.push_back(d.connection[0] - 1); eles.push_back(d.connection[1] - 1); eles.push_back(d.connection[3] - 1);
					eles.push_back(d.connection[0] - 1); eles.push_back(d.connection[1] - 1); eles.push_back(d.connection[4] - 1);
					eles.push_back(d.connection[1] - 1); eles.push_back(d.connection[2] - 1); eles.push_back(d.connection[5] - 1);
					eles.push_back(d.connection[4] - 1); eles.push_back(d.connection[5] - 1); eles.push_back(d.connection[7] - 1);
					eles.push_back(d.connection[0] - 1); eles.push_back(d.connection[4] - 1); eles.push_back(d.connection[3] - 1);
					eles.push_back(d.connection[2] - 1); eles.push_back(d.connection[3] - 1); eles.push_back(d.connection[6] - 1);

					eles.push_back(d.connection[1] - 1); eles.push_back(d.connection[2] - 1); eles.push_back(d.connection[3] - 1);
					eles.push_back(d.connection[1] - 1); eles.push_back(d.connection[5] - 1); eles.push_back(d.connection[4] - 1);
					eles.push_back(d.connection[2] - 1); eles.push_back(d.connection[6] - 1); eles.push_back(d.connection[5] - 1);
					eles.push_back(d.connection[5] - 1); eles.push_back(d.connection[6] - 1); eles.push_back(d.connection[7] - 1);
					eles.push_back(d.connection[4] - 1); eles.push_back(d.connection[7] - 1); eles.push_back(d.connection[3] - 1);
					eles.push_back(d.connection[3] - 1); eles.push_back(d.connection[7] - 1); eles.push_back(d.connection[6] - 1);
				}
				free(elements);
			}
			element.close();
		}
		return eles;
	}
	vector<float> GetNodesDisplacement(int com) const {
		vector<float> dis;
		DataSet dis_set = m_h5data->openDataSet(DISPLACEMENT_PATH);
		CompType dises = dis_set.getCompType();
		hsize_t length;
		dis_set.getSpace().getSimpleExtentDims(&length);
		DISPLACEMENT *res = new DISPLACEMENT[length];
		dis_set.read(res, dises);
		for (int i = 0; i < length; i++) {
			switch (com) {
			case 0:
				dis.push_back(res[i].x);
				break;
			case 1:
				dis.push_back(res[i].y);
				break;
			case 2:
				dis.push_back(res[i].z);
				break;
			case -1:
				dis.push_back(sqrtf(res[i].x*res[i].x+ res[i].y*res[i].y+ res[i].z*res[i].z));
				break;
			}
		}
		free(res);
		dis_set.close();
		return dis;
	}

private:
	string m_file;
	H5File * m_h5data;
};

