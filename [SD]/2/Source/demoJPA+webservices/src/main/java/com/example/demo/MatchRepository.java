package com.example.demo;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;

import java.util.List;

import com.example.data.Match;

public interface MatchRepository extends CrudRepository<Match, Integer> {
    @Query("select p from Player p where p.name like %?1")
    public List<Match> findByNameEndsWith(String chars);
}    