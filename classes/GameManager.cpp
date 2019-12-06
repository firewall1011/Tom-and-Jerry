#include "Entity.h"
#include "Cat.h"
#include "Mouse.h"
#include "GameManager.h"

namespace GameManager {
    //Entity params
    int mutation_rate = 10;
    long long iter = 0;
    float speed_mutation = 1.0f;
    float smell_range_mutation = 0.5f;
    float min_speed = 1.0f;
    float min_smell_range = 2.0f;

    // GA params
    int genNum = 1;
    struct best {
        long long surv_time;
        int sml_rng[2];
        float spd[2];
        float rep_lim[2];
    } best_params;
    int no_improvement = 0;

    // Cat init params
    int cat_population = 10;
    float cat_smell_range = 5;
    float cat_speed = 1;
    float cat_reproduction_limiar = 0.6;

    // Mouse init params
    int mouse_population = 50;
    float mouse_smell_range = 5;
    float mouse_speed = 1;
    float mouse_reproduction_limiar = 0.6;

    // Food init params
    int food_amount = 150;
    int food_spawn_difficulty = 15;

    // Entities
    vector<Cat*> cats;
    vector<Mouse*> mice;
    vector<Food*> foods;
    
    Cat* reproduct(Cat* c1, Cat* c2){
        Cat* child = new Cat(0, 0, c1->reproduction_limiar);

        child->smell_range = (c1->smell_range + c2->smell_range)/2;
        child->speed = (c1->speed + c2->speed)/2;
        //child->reproduction_limiar = (c1->reproduction_limiar + c2->reproduction_limiar)/2; // keeping the same limiar to every cat
        child->pos = c1->pos;

        mutate(child);

        return child;
    }

    Mouse* reproduct(Mouse* m1, Mouse* m2){
        Mouse* child = new Mouse(0, 0, m1->reproduction_limiar);

        child->smell_range = (m1->smell_range + m2->smell_range)/2;
        child->speed = (m1->speed + m2->speed)/2;
        //child->reproduction_limiar = (m1->reproduction_limiar + m2->reproduction_limiar)/2; // keeping the same limiar to every mouse
        child->pos = m1->pos;

        mutate(child);

        return child;
    }

    void mutate(Cat* cat){
        cat->speed +=   (!(rand() % mutation_rate)) ? 
                            ((rand() % 2) && cat->speed > speed_mutation ? 
                                -speed_mutation 
                                : speed_mutation) 
                            : 0;
        cat->smell_range +=     (!(rand() % mutation_rate)) ? 
                                    ((rand() % 2) && cat->smell_range > smell_range_mutation ? 
                                        -smell_range_mutation  
                                        : smell_range_mutation) 
                                    : 0;
    }

    void mutate(Mouse* mouse){
        mouse->speed +=     (!(rand() % mutation_rate)) ? 
                                ((rand() % 2) && mouse->speed > speed_mutation ? 
                                    -speed_mutation  
                                    : speed_mutation) 
                                : 0;
        mouse->smell_range +=   (!(rand() % mutation_rate)) ? 
                                    ((rand() % 2) && mouse->smell_range > smell_range_mutation ? 
                                        -smell_range_mutation  
                                        : smell_range_mutation) 
                                    : 0;
    }

    void initPop() {
        cats.clear();
        mice.clear();
        foods.clear();

        for(int i = 0; i < cat_population; i++){
            Cat* cat = new Cat(cat_smell_range, cat_speed, cat_reproduction_limiar);
            cat->childhood = 0;
            cat->pos.x = WIDTH-2;
            cat->pos.y = HEIGHT-2;
            cats.push_back(cat);
        }

        for(int i = 0; i < mouse_population; i++){
            Mouse* mouse = new Mouse(mouse_smell_range, mouse_speed, mouse_reproduction_limiar);
            mouse->childhood = 0;
            mouse->pos.x = (int)(WIDTH/2);
            mouse->pos.y = (int)(HEIGHT/2);
            mice.push_back(mouse);
        }

        for(int i = 0; i < food_amount; i++){
            Food* food = new Food(0, 0, 0);
            food->pos = Vector2(rand()%WIDTH, rand()%HEIGHT);
            foods.push_back(food);
        }
        
        cout << "Generation " << genNum << " started with " << cat_population << " cats and " << mouse_population << " mice." << endl;
    }

    void makeStep() {

        for(int i = 0; i < mice.size(); i++){
            Mouse* m = mice[i];

            m->calculateReproductionUrge(1, 1);
            m->CheckRadar(cats, foods, mice);
            m->move(WIDTH, HEIGHT);

            // if is dead (energy = 0), delete
            if(m->energyConsume()){
                mice.erase(mice.begin() + i);
                i--;
            }
        }
        
        for(int i = 0; i < cats.size(); i++){
            Cat* c = cats[i];

            c->calculateReproductionUrge(1, 1);
            c->CheckRadar(mice, cats);
            c->move(WIDTH, HEIGHT);

            // if is dead (energy = 0), delete
            if(c->energyConsume()){
                cats.erase(cats.begin() + i);
                i--;
            }
        }

        iter++;

    }

    float calculateWeight() {
        return log(no_improvement + 1);
    }

    void mutate() {
        float weight = calculateWeight();
        // cout << "c_spd bef mut: " << cat_speed << endl;
        // cout << "m_spd bef mut: " << mouse_speed << endl;
        cat_reproduction_limiar += rand() % 2 && cat_reproduction_limiar > 0.01 * weight ? -0.01 * weight : 0.01 * weight;
        cat_speed += rand() % 2 && cat_speed > min_speed + 0.1 * weight ? -0.1 * weight : 0.1 * weight;
        cat_smell_range += rand() % 2 && cat_smell_range > min_smell_range + 0.25 * weight ? -0.25 * weight : 0.25 * weight;
        mouse_reproduction_limiar += rand() % 2 && mouse_reproduction_limiar > 0.01 * weight ? -0.01 * weight : 0.01 * weight;
        mouse_speed += rand() % 2 && mouse_speed > min_speed + 0.1 * weight ? -0.1 * weight : 0.1 * weight;
        mouse_smell_range += rand() % 2 && mouse_smell_range > min_smell_range + 0.25 * weight ? -0.25 * weight : 0.25 * weight;
        return;
    }

    void makeNewGen() {
        srand(time(NULL));
        
        cout << "Generation " << genNum++ << " survived for " << iter << " iterations. Best is " << best_params.surv_time << "." << endl;

        if(iter > best_params.surv_time){
            cout << "Params improved." << endl;
            cout << "Cat stats: " << endl;
            cout << "\tsmrange: " << cat_smell_range << endl;
            cout << "\tspeed:   " << cat_speed << endl;
            cout << "\treprlim: " << cat_reproduction_limiar << endl;
            cout << "Mouse stats: " << endl;
            cout << "\tsmrange: " << mouse_smell_range << endl;
            cout << "\tspeed:   " << mouse_speed << endl;
            cout << "\treprlim: " << mouse_reproduction_limiar << endl;
            best_params.surv_time = iter;
            best_params.sml_rng[0] = cat_smell_range;
            best_params.sml_rng[1] = mouse_smell_range;
            best_params.spd[0] = cat_speed;
            best_params.spd[1] = mouse_speed;
            best_params.rep_lim[0] = cat_reproduction_limiar;
            best_params.rep_lim[1] = mouse_reproduction_limiar;
            no_improvement = 0;
        } else {
            cout << "No improvement." << endl;

            cat_smell_range = best_params.sml_rng[0];
            mouse_smell_range = best_params.sml_rng[1];
            cat_speed = best_params.spd[0];
            mouse_speed = best_params.spd[1];
            cat_reproduction_limiar = best_params.rep_lim[0];
            mouse_reproduction_limiar = best_params.rep_lim[1];
            no_improvement++;
        }
        
        cout << endl << endl;
        
        mutate();

        iter = 0;
        srand(RAND_SEED);
        initPop();
    }

    void genFood() {
        if(!(rand() % food_spawn_difficulty)) {
            Food* food = new Food(0, 0, 0);
            food->pos = Vector2(rand()%WIDTH, rand()%HEIGHT);
            foods.push_back(food);
        }
    }

    bool genEnded(){
        return (cats.size() <= 1 || mice.size() <= 1);
    }

}