#pragma once

#include <string>
#include <vector>
#include <algorithm>

namespace ctrl01 {

struct WorkerInfo {
  std::string name;
  int story_points;

  bool operator<(const WorkerInfo& rhs) const {
    return name < rhs.name;
  }
};


std::vector<std::pair<std::string, int>>
GetTopWorkers(const std::vector<std::string> &workers,
              const std::vector<WorkerInfo> &workers_info,
              size_t n) {
    std::vector<std::pair<std::string, int>> result;
    std::vector<WorkerInfo> tmp(workers_info);
    std::sort(tmp.begin(), tmp.end(), [](const WorkerInfo& l, const WorkerInfo& r) {
        return l.story_points > r.story_points;
    });
    size_t counter = 0;
    for (auto it = tmp.cbegin(); it != tmp.cend() && counter < n; ++it, ++counter) {
        bool check = std::binary_search(workers.cbegin(), workers.cend(), it->name, [](const std::string& l,
                                                                                       const std::string& r) {
            return std::lexicographical_compare(l.cbegin(), l.cend(), r.cbegin(), r.cend()) &&
                   l.size() <= r.size();
        });
        if (check) {
            result.emplace_back(it->name, it->story_points);
        }
    }
    return result;
}


template<typename ForwardIterator, typename OutputIterator, typename Equal,
    typename Merge>
void CollapseSequential(ForwardIterator first, ForwardIterator last,
                        OutputIterator out, Equal equal, Merge merge);

struct Task {
  std::string project;
  std::string assignee;
  int spent_sp;
};


struct WorkerResult {
  std::string worker_name;
  int story_points;

  bool operator==(const WorkerResult &rhs) const {
    return worker_name == rhs.worker_name
        && story_points == rhs.story_points;
  }
};


struct ProjectResult {
  std::string project_name;
  std::vector<WorkerResult> workers_results;

  bool operator<(const ProjectResult &rhs) const {
    return project_name < rhs.project_name;
  }

  bool operator==(const ProjectResult &rhs) const {
    return project_name == rhs.project_name
        && workers_results == rhs.workers_results;
  }
};

std::vector<ProjectResult> GetTopPerformersByProject(std::vector<Task> tasks);

}