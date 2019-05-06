#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <set>

//#define TEST

class Compare{																			//Класс-компоратор для сортировки пар в очереди с приоритетом
public:
	bool operator()(const std::pair<char, char> &a,const std::pair<char, char> &b) const
	{
		return abs(a.first-a.second)>abs(b.first-b.second);
	}
};


typedef std::priority_queue <std::pair<char, char>,
		std::vector < std::pair<char, char>>,
		Compare> queue;									//очередь с приоритетом

std::set<std::pair<char,char>> input;					//ребра, поданные на вход

class Graph{
	std::vector<std::vector<std::pair<int,int>>> matr;  //матрица смежности
	std::vector<char> v;
	int next_track(char from, char to){					//метод, который ищет путь и возвращает поток, проходящий через него
		queue q;
		std::set<char> checked;							//список посещенных вершин
		std::map<char,char> track;						//текущий путь, хранящий пары: "куда", "откуда"
		char cur=from;
		checked.insert(cur);
		find_next(q,checked,from, to);
#ifdef TEST
		print_matr();
		print_queue(q);
#endif
		while(!q.empty()){									//пока существуют ребра, по которым можно пройти
			cur=q.top().second;
#ifdef TEST
			std::cout<<"Текущий переход: "<<q.top().first<<"->"
			<<q.top().second<<std::endl;
#endif
			checked.insert(cur);
			track[cur]=q.top().first;
			q.pop();
			if(cur==to){
#ifdef TEST
				std::cout<<"Найден конец!\n";
#endif
				int min=find_min_flow(track,from, to);			//находим поток
				recount_flow(track, min, from, to);				//делаем перерасчет проходящего потока
				return min;
			}
			find_next(q,checked,cur, to);
#ifdef TEST
			if(q.empty()){
				std::cout<<"Путей больше нет!\n";
				continue;
			}
			print_queue(q);
#endif
		}
		return 0;
	}

public:
	Graph(){}
	void add(char from, char to, int flow){						//метод добавления ребра в граф
		int fr,t;
		fr=get_index(from);
		if(fr==-1)
			v.push_back(from);
		t=get_index(to);
		if(t==-1)
			v.push_back(to);
		if(matr.size()<v.size()){
			matr.resize(v.size());
			for(int i=0;i<matr.size();i++){
				matr[i].resize(matr.size());
			}
		}
		matr[get_index(from)][get_index(to)]={flow,0};
	}
	int get_index(char el){										//получение индекса вершины по ее букве
		for(int i=0;i<v.size();i++){
			if(v[i]==el)
				return i;
		}
		return -1;
	}
	void print_matr(){											//метод печати матрицы
		std::cout<<"   ";
		for(int i=0;i<v.size();i++)
			std::cout<<v[i]<<"   ";
		std::cout<<std::endl;
		for(int i=0;i<matr.size();i++){
			std::cout<<v[i]<<" ";
			for(int j=0;j<matr[i].size();j++)
				std::cout<<matr[i][j].first<<"/"<<matr[i][j].second<<' ';
			std::cout<<std::endl;
		}
	}
	int get_resid_flow(char from, char to){							//получение максимально возможного потока через данное ребро на текущем этапе
		if(get_index(from)==-1 || get_index(to)==-1)
			return -1;
		return matr[get_index(from)][get_index(to)].first-matr[get_index(from)][get_index(to)].second;
	}

	void find_next(queue &q,std::set<char> &checked, char el,char to){			//метод ищет инцидентные вершины, которые не были посещены и опускает их в очередь
		if(get_resid_flow(el,to)>0){
			while(!q.empty()){
				q.pop();
			}
			q.push({el,to});
			return;
		}
		for(int k=0;k<v.size();k++){

			if(get_resid_flow(el,v[k])<=0 || checked.find(v[k])!=checked.end())
				continue;
			q.push({el,v[k]});
			checked.insert(v[k]);
		}
	}
	void recount_flow(std::map<char,char> &track, int flow, char from, char to){				//метод проходит по пути и пересчитывает проходящий через ребра поток
		char cur=to;
#ifdef TEST
		std::cout<< "Пересчет потока, проходящего через ребра найденного пути.\n";
#endif
		while(cur!=from){
			matr[get_index(track[cur])][get_index(cur)].second+=flow;
			matr[get_index(cur)][get_index(track[cur])].second-=flow;
			cur=track[cur];
		}
	}

	int find_min_flow(std::map<char, char> &track,char from, char to){							//метод поиска минимального потока в пути
		int min=get_resid_flow(track[to],to);
		char cur=track[to];
#ifdef TEST
		std::string str;
		str.push_back(cur);
		str.push_back(to);
#endif
		while(cur!=from){
			if(get_resid_flow(track[cur],cur)<min)
				min=get_resid_flow(track[cur],cur);
			cur=track[cur];
#ifdef TEST
			str.insert(str.begin(),cur);
#endif
		}
#ifdef TEST
			std::cout<<"Путь: "<<str<<std::endl
			<<"Поток пути: "<<min<<std::endl;
#endif
		return min;
	}

	void find_flow(char from, char to){ 								//метод поиска максимального потока в графе
#ifdef TEST
		int i=2;
		std::cout<<"Начинаем алгоритм!\n";
#endif
		int max_gr_flow=0,min_track_flow=0;
#ifdef TEST
		std::cout<<"Путь #1\n";
#endif
		min_track_flow=next_track(from, to);
		while(min_track_flow>0){								//пока существует путь
#ifdef TEST
		std::cout<<"Путь #"<<i<<std::endl;
		i++;
#endif
			max_gr_flow+=min_track_flow;					//увеличиваем максимальный поток на величину потока пути
			min_track_flow=next_track(from, to);
		}
		std::cout<<max_gr_flow<<std::endl;
		print_flows();
	}
	void print_flows(){										//метод печати реальных потоков в изначальных ребрах
		for(auto k:input){
			if(matr[get_index(k.first)][get_index(k.second)].second>=0)
				std::cout<<k.first<<' '<<k.second<<' '<<matr[get_index(k.first)][get_index(k.second)].second<<std::endl;
			else
				std::cout<<k.first<<' '<<k.second<<' '<<0<<std::endl;
		}
	}
	void print_queue(queue q){								//метод печати очереди с приоритетом
		std::cout<<"Ребра в очереди:\n";
		while(!q.empty()){
			std::cout<<q.top().first<<"->"<<q.top().second<<std::endl;
			q.pop();
		}
		std::cout<<std::endl;
	}
};

int main(){
	Graph a;
	int n,flow;
	char from,to,first,second;
	std::cin>>n>>from>>to;
	for(int i=0;i<n;i++){
		std::cin>>first>>second>>flow;
		input.insert(std::pair<char,char>(first,second));
		a.add(first,second,flow);
	}
	a.find_flow(from,to);
	return 0;
}