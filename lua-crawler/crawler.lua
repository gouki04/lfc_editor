require 'utility'

module('crawler', package.seeall)

base_dir = nil
lfc_team_id = 53
crawled_matches = nil
crawled_playeres = nil

function init(dir_path)
    base_dir = dir_path
    crawled_matches = utility.load_table_from_file(dir_path .. '/crawled_matches.lua') or {}
    crawled_playeres = utility.load_table_from_file(dir_path .. '/crawled_playeres.lua') or {}
end

function load_html(year, id)
    local file_name = table.concat({base_dir, '/', year, '/', id, '.html'})
    local file = io.open(file_name, 'r')
    if file ~= nil then
        local file_content = file:read('*a')
        file:close()
        
        return file_content
    end
    
    return nil
end

function crawl_match_by_year(year)
    local http = require 'socket.http'
    local url = string.format('http://app.gooooal.com/teamAllMatch.do?sid=%d&tid=%d&t=0&lang=cn', year, lfc_team_id)
    local content = http.request(url)
    
    content = string.gsub(content, '<span[^>]+>', '')
    
    local not_crawl_matches = {}
    string.gsub(content, '<td>(%d%d%d%d%-%d%d%-%d%d)</td>[^:]+:toTeam%((%d+),%d+%)"%s*>([^<]+)<[^:]+:toEvents%((%d+)%)"%s*>([^<]+)<[^:]+:toTeam%((%d+),%d+%)"%s*>([^<]+)<', 
        function(date, home_team_id, home_team, match_id, result, away_team_id, away_team)
            match_id = tonumber(match_id)
            home_team_id = tonumber(home_team_id)
            away_team_id = tonumber(away_team_id)
        
            if crawled_matches[match_id] or not_crawl_matches[match_id] then
                return
            end
        
            local opponent_team, side
            if home_team_id == lfc_team_id then
                opponent_team = {
                    id = away_team_id,
                    name = away_team,
                }
                side = 'home'
            else
                opponent_team = {
                    id = home_team_id,
                    name = home_team,
                }
                side = 'away'
            end
            
            not_crawl_matches[match_id] = {
                id = match_id,
                date = date,
                year = year,
                side = side,
                opponent_team = opponent_team,
                result = string.format('%s_%s_%s_%s', date, home_team, result, away_team),
            }
        end)
    
    local lfs = require 'lfs'
    local dir = base_dir .. '/' .. year
    lfs.mkdir(dir)
    
    for id, match in pairs(not_crawl_matches) do
        local url = string.format('http://app.gooooal.com/sports/football/analysis/event.do?matchId=%d&lang=cn', id)
        print ('crawling match id == ' .. id)
        
        local match_content = http.request(url)
        local file_name = dir .. '/' .. id .. '.html'
        local f, err = io.open(file_name, 'w')
        f:write(match_content)
        f:flush()
        f:close()
        
        crawled_matches[id] = match
    end
    
    utility.save_table_to_file(crawled_matches, base_dir .. '/crawled_matches.lua')
    print ('save crawled_matches')
end

function crawl_player_by_year(year, max_cnt)
    local parsed_matches = parser.parsed_matches
    for id, match in pairs(parsed_matches) do
        if match.year == year then
            local match_t = parser.load_json(year, id)
            if match_t then
                for i,v in ipairs(match_t.opponent_player) do
                    local pid = v.pid
                    if crawled_playeres[pid] then
                        -- continue
                    else
                        
                    end
                end
            end
        end
    end
end