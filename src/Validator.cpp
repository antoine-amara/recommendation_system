#include "Validator.h"

using namespace std;

Validator::Validator(int N) {
	this->m_filename = "u";
	this->m_nbTestSets = 5;
	this->m_N = N;
}

Validator::Validator(string filename, int nbTestSets, int N) {
	this->m_filename = filename;
	this->m_nbTestSets = nbTestSets;
	this->m_N = N;
}

void Validator::start(){
	cout << "##############################" << endl;
	cout << "# Start Test Session         #" << endl;
	cout << "##############################" << endl;
	cout << endl;
	cout << "dataset name: " << m_filename << endl;
	cout << "number of dataset: " << m_nbTestSets << endl;
	cout << "entry per dataset: " << m_N << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	for(int i=0; i < this->m_nbTestSets; i++){
		string name = m_filename+to_string(i+1);
		computeError(name);
	}
	printReport();
	m_errors.clear();
}

void Validator::startRMSE() {
	cout << "##############################" << endl;
	cout << "#     Start Test Session     #" << endl;
	cout << "##############################" << endl;
	cout << endl;
	cout << "dataset name: " << m_filename << endl;
	cout << "number of dataset: " << m_nbTestSets << endl;
	cout << "entry per dataset: " << m_N << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	for(int i=0; i < this->m_nbTestSets; i++){
		string name = m_filename+to_string(i+1);
		computeRMSE(name);
	}
	printReport();
	m_errors.clear();
}



void Validator::computeError(string dataset){
	MovieRecommender *movieRecommender = new MovieRecommender(Saver(dataset));
	DataParser* d = new DataParser(dataset, movieRecommender->getX()->size1, movieRecommender->getTheta()->size1);
	Vector3 dTest[m_N];
	d->parseTest(dTest, m_N);
	int errorCount = 0;
	gsl_matrix *normalizerates;

	normalizerates = movieRecommender->normalize();

	for(int i = 0; i < m_N; i++){
		int mark = gsl_matrix_get(normalizerates,dTest[i].x(),dTest[i].y());
		if(mark != dTest[i].z())
			errorCount++;
	}

	m_errors.push_back((errorCount+0.0) * 100/m_N+0.0);

	gsl_matrix_free(normalizerates);
	delete(movieRecommender);
	delete(d);
}

void Validator::computeRMSE(string dataset) {
	MovieRecommender *movieRecommender = new MovieRecommender(Saver(dataset));
	DataParser* d = new DataParser(dataset, movieRecommender->getX()->size1, movieRecommender->getTheta()->size1);
	movieRecommender->setTestDatas(dataset, movieRecommender->getX()->size1, movieRecommender->getTheta()->size1, m_N);

	m_errors.push_back(movieRecommender->computeCost(TESTSET, 5/100));

	delete(movieRecommender);
	delete(d);
}

double Validator::computeGlobalError(){
	double taille = this->m_errors.size();
	double GlobalError = accumulate(this->m_errors.begin(), this->m_errors.end(),0.0);
	GlobalError /= taille;

	return GlobalError;
}

void Validator::printReport(){
	cout << "##############################" << endl;
	cout << "#        Test Report:        #" << endl;
	cout << "##############################" << endl;
	cout << endl;
	for(unsigned int i = 0; i < this->m_errors.size(); i++){
		cout << "Dataset name :" << m_filename+to_string(i+1);
		cout << " Error :" << to_string(this->m_errors[i]) << endl;
	}
	cout << endl;
	cout << "GlobalError :" << to_string(computeGlobalError()) << endl;
	cout << endl;
	cout << "##############################" << endl;

}

Validator::~Validator(){
}
