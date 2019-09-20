#include <bits/stdc++.h>
using namespace std;

#define MAX_POP 40
#define INIT_POP 20

//first
vector<float> initPop(int amt) {
	vector<float> pop;
	
	for (int i = 0; i < amt; i++) {
		float vals = ((rand() % 1000) / 10.0) - 30.0;
		pop.push_back(vals);
	}

	return pop;
}

//second
float evaluate(float x) {
	//f(c) =| x, if c <= 0.5
	//		| 0.5 - x, if c > 0.5
	// return c <= 0.5 ? c : 1.0 - c;
	return 2*cos(0.39*x) + 5*sin(0.5*x) + 0.5*cos(0.1*x) + 10*sin(0.7*x) + 5*sin(1*x) + 5*sin(0.35*x);
}

//third
int selection(vector<float> & pop, float fit[]) {
	//a) best with others
	//b) roulette
	//c) 2 tournment
		// 2 rands, fight, choose father
		// 2 rands, fight, choose mother
		// make child
	int best = 0;
	for (int i = 0; i < pop.size(); i++) {
		if (fit[best] < fit[i]) best = i;
	}

	return best;
}

//fourth
int crossover(vector<float> & pop, int best) {
	//i) choose rand between father or mother gene
	//ii) aritmethic average
	int size = pop.size();
	for (int i = 0; i < size; i++) {
		if (i == best) continue;
		float child = (pop[i] + pop[best]) / 2.0;
		// cout << pop[i] << " + " << pop[best] << " = " << child << endl;
		pop.push_back(child);
	}

	return size;
}

//fifth
void mutate(vector<float> & pop, float fitBest, float fitLastBest, int parents, int noChange) {
	// first - mutate all
		// which gene to mutate (just one) 
			// amount to mutate +/- 0.01
	
	//before
	// for (int i = parents - 1; i < pop.size(); i++) {
	// 	float mut = 100.0 / ((rand() / 100) + 1.0);
	// 	pop[i] += (!(rand() % 10)) ? (rand() % 2 ? mut  : -mut) : 0;
	// }
	for (int i = parents; i < pop.size(); i++) {
		float w = (noChange * noChange);
		float mut = (parents == INIT_POP || (fabs(fitBest - fitLastBest) <= 0.01)) ? 10.0 * w : 0.01;
		pop[i] += (!(rand() % 10)) ? (rand() % 2 ? mut  : -mut) : 0;
	}
}

//sixth
void rearrange(vector<float> & pop, int best, int parents) {
	//kill some, but the best, put others (children generated)
	
	float valOfBest = pop[best];
	pop.erase(pop.begin() + best);

	while (pop.size() > MAX_POP - 1) {
		pop.erase(pop.begin() + (rand() % parents));
	}

	pop.push_back(valOfBest);
}

int main () {
	float fit[MAX_POP];

	srand(time(NULL));
	vector<float> pop = initPop(INIT_POP);


	int turn = 0;
	int noChange = 0;
	int best;
	float fitLastBest;
	
	cout << setprecision(6) << fixed;
	cout << "gen, best, avg\n";

	while (noChange < 100) {


		if (turn != 0) fitLastBest = fit[pop.size() - 1];
		// cout << "fitness: \n";
		for (int i = 0; i < pop.size(); i++) {
			fit[i] = evaluate(pop[i]);
			// cout << "\t" << pop[i] <<"(" << fit[i] << ")\n";
		}
		// cout << endl;

		float avg = 0;
		for (int i = 0; i < pop.size(); i++)
			avg += fit[i];
		avg /= pop.size();

		float delta = 0;
		for (int i = 0; i < pop.size(); i++)
			delta += fabs(fit[i] - avg);
		delta /= pop.size();
		
		best = selection(pop, fit);
		int parents = crossover(pop, best);
		mutate(pop, fit[best], fitLastBest, parents, noChange);
		rearrange(pop, best, parents);

		cout << setfill('0') << setw(5) << turn + 1 << ", " << fit[best] << ", " << avg <<  "\n";

		if (turn != 0 && fabs(fit[best] - fitLastBest) <= 0.01) noChange++;
		if (fabs(fit[best] - fitLastBest) > 0.01) noChange = 0;

		turn++;
	}

	return 0;
}