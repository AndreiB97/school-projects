#include <iterator>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <cmath>
#include "imdb.h"

std::string floatToString(float f) {
    std::string s;
    f = round(f*100)/100;
    s = std::to_string(f);
    for(unsigned int i = 0 ; i < s.length(); i++) {
        if (s[i] == '.') {
            i += 3;
            s.erase(i, s.length());
        }
    }
    return s;
}

IMDb::IMDb (  )  {
    // initialize what you need here.
}

IMDb::~IMDb (  )  {}

void IMDb::add_movie ( std::string movie_name,
                     std::string movie_id,
                     int timestamp, // unix timestamp when movie was launched
                     std::vector<std::string> categories,
                     std::string director_name,
                     std::vector<std::string> actor_ids )  {
    std::map<std::string, actor_info<std::string> >::iterator actor_it;
    movie_info<std::string> movie ( movie_name, timestamp, director_name, categories, actor_ids ) ;
    std::vector<std::string> aux;
    for (unsigned int i = 0; i < actor_ids.size(); i++) {
        aux = actor_ids;
        aux[i] = aux[aux.size()-1];
        aux.erase(aux.end()-1);
        actor_it = actors.find(actor_ids[i]);
        actor_it->second.movies.push_back(movie_id);
        for (unsigned int j = 0; j < aux.size(); j++) {
            actor_it->second.actors.push_back(aux[j]);
        }
    }
    movies.insert ( std::pair<std::string, movie_info<std::string> > ( movie_id, movie ) );
}

void IMDb::add_user ( std::string user_id, std::string name )  {
    users.insert ( std::pair<std::string, user_info<std::string> > ( user_id, user_info<std::string>( name )  )  ) ;
}

void IMDb::add_actor ( std::string actor_id, std::string name )  {
    actors.insert ( std::pair<std::string, actor_info<std::string> > ( actor_id, actor_info<std::string> ( name )  )  ) ;
}

void IMDb::add_rating ( std::string user_id, std::string movie_id, int rating )  {
    std::map<std::string, user_info<std::string> >::iterator user_it;
    std::map<std::string, int>::iterator rating_it;
    std::map<std::string, movie_info<std::string> >::iterator movie_it;
    user_it = users.find(user_id);
	// verifica daca exista deja un rating
    rating_it = user_it->second.ratings.find(movie_id);
	// daca nu exista este adaugat
    if (rating_it == user_it->second.ratings.end()) {
        user_it->second.ratings.insert(std::pair<std::string, int>(movie_id, rating));
        movie_it = movies.find(movie_id);
        movie_it->second.totalRatings += rating;
        movie_it->second.nrRatings++;
    }

}

void IMDb::update_rating ( std::string user_id, std::string movie_id, int rating )  {
    std::map<std::string, user_info<std::string> >::iterator user_it;
    std::map<std::string, int>::iterator rating_it;
    std::map<std::string, movie_info<std::string> >::iterator movie_it;

    user_it = users.find(user_id);
	// verifica daca exista ratingul
    rating_it = user_it->second.ratings.find(movie_id);
	// daca exista il actualizeaza
    if (rating_it != user_it->second.ratings.end()) {
        movie_it = movies.find(movie_id);
        movie_it->second.totalRatings -= rating_it->second;
        movie_it->second.totalRatings += rating;
        rating_it->second = rating;
    }
}

void IMDb::remove_rating ( std::string user_id, std::string movie_id )  {
    std::map<std::string, user_info<std::string> >::iterator user_it;
    std::map<std::string, int>::iterator rating_it;
    std::map<std::string, movie_info<std::string> >::iterator movie_it;

    user_it = users.find(user_id);
	// verifica daca exista ratingul
    rating_it = user_it->second.ratings.find(movie_id);
	// daca exista il sterge
    if (rating_it != user_it->second.ratings.end()) {
        movie_it = movies.find(movie_id);
        movie_it->second.totalRatings -= rating_it->second;
        movie_it->second.nrRatings--;
        user_it->second.ratings.erase(rating_it);
    }
}

std::string IMDb::get_rating ( std::string movie_id )  {
    std::map<std::string, movie_info<std::string> >::iterator movie_it;

    float r;

    movie_it = movies.find(movie_id);
	// daca filmul are cel putin un rating afiseaza nota
    if (movie_it->second.nrRatings != 0) {
        r = movie_it->second.totalRatings / (float)movie_it->second.nrRatings;
    } else {
        return "none";
    }
    return floatToString(r);
}

std::string IMDb::get_longest_career_actor (  )  {
    std::map<std::string, actor_info<std::string> >::iterator actor_it;
    std::map<std::string, movie_info<std::string> >::iterator movie_it;
	std::string id_long_actor = "none";
	int max_career_length = -1;
	int first, last;
	for (actor_it = actors.begin(); actor_it != actors.end(); actor_it++) {
		first = last = movies.find(*(actor_it->second.movies.begin()))->second.data;
		for (std::list<std::string>::iterator str_it = actor_it->second.movies.begin(); str_it != actor_it->second.movies.end(); str_it++) {
			if (movies.find(*(str_it))->second.data < first) {
				first = movies.find(*(str_it))->second.data;
			}
			if (movies.find(*(str_it))->second.data > last) {
				last = movies.find(*(str_it))->second.data;
			}
		}
		if (last-first > max_career_length) {
			max_career_length = last-first;
			id_long_actor = actor_it->second.name;
		} else if (last-first == max_career_length) {
			if (id_long_actor > actor_it->second.name) {
				id_long_actor = actor_it->second.name;
			}
		}
	}
    return id_long_actor;
	return "none";
}

std::string IMDb::get_most_influential_director (  )  {
    return "";
}

std::string IMDb::get_best_year_for_category ( std::string category )  {
    return "";
}

std::string IMDb::get_2nd_degree_colleagues ( std::string actor_id )  {
    return "";
}

std::string IMDb::get_top_k_most_recent_movies ( int k )  {
    return "";
}

std::string IMDb::get_top_k_actor_pairs ( int k )  {
    return "";
}

std::string IMDb::get_top_k_partners_for_actor ( int k, std::string actor_id )  {
    return "";
}

std::string IMDb::get_top_k_most_popular_movies ( int k )  {
    return "";
}

std::string IMDb::get_avg_rating_in_range ( int start, int end )  {
    return "";
}
