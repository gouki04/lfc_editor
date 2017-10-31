require 'utility'

module('parser', package.seeall)

base_dir = nil
lfc_team_id = 53
parsed_matches = nil
parsed_playeres = nil

function init(dir_path)
    base_dir = dir_path
    parsed_matches = utility.load_json_from_file(base_dir .. '/parsed_matches.json') or {}
    parsed_playeres = utility.load_json_from_file(base_dir .. '/parsed_playeres.json') or {}
end

function load_json(year, id)
    local file_name = table.concat({base_dir, '/', year, '/', id, '.html'})
    local file = io.open(file_name, 'r')
    if file ~= nil then
        local file_content = file:read('*a')
        file:close()
        
        local json = require 'cjson'
        return json.decode(file_content)
    end
    
    return nil
end

function parser_match_by_year(year)
    local crawled_matches = crawler.crawled_matches
    for id, match in pairs(crawled_matches) do
        local id_str = tostring(id)
        if parsed_matches[id_str] or match.year ~= year then
            -- continue
        else
            print ('parsing match id == ' .. id)
            
            local file_content = crawler.load_html(year, id)
            if file_content ~= nil then
                local match_t = parse_match(id, file_content)
                
                local lfs = require 'lfs'
                local dir = base_dir .. '/' .. year
                lfs.mkdir(dir)
                
                utility.save_json_to_file(match_t, table.concat({dir, '/', id, '.json'}))
                parsed_matches[id_str] = match
            else
                print (string.format('[parse error]cannot load html, year = %d, id = %d', year, id))
            end
        end
    end
    
    utility.save_json_to_file(parsed_matches, base_dir .. '/parsed_matches.json')
    print ('save parsed_matches')
end

-- logId
-- 1 进球 assistId 助攻球员
-- 2 乌龙
-- 3 点球（罚进）
-- 104 换人（下场）
-- 105 换人（上场） id和104对应
-- 106 受伤
-- 202 黄牌
-- 203 红牌
-- 205 罚丢点球

-- periodId
-- 100 上半场
-- 102 下半场
-- 103 加时赛
-- 104 点球大战

function parse_match(match_id, match_str)
    local side = 'home'
    local lfc_player = {}

    local opponent_team = nil
    local opponent_player = {}

    local date = nil
    local lfc_goals = {}
    local opponent_goals = {}

    local content = match_str

    --[[
    开赛时间 2017年09月09日
    ]]
    string.gsub(content, '开赛时间 (%d+)年(%d+)月(%d+)日', function(y,m,d)
          date = {
              year = tonumber(y),
              month = tonumber(m),
              day = tonumber(d),
            }
        end)
      
    --[[
    var _hostTeam = ["216","曼城","http://pic.gooooal.com/images/100838/100838782.png","123339","Manchester City FC","1"];
    ]]
    string.gsub(content, 'var%s+_hostTeam%s+=%s%[([^;]+)%]', function(str)
          local t = loadstring('return {' .. str .. '}')()
          if t[1] == tostring(lfc_id) then
            side = 'home'
          else
            opponent_team = {
                id = tonumber(t[1]),
                name = t[2],
                en_name = t[5],
              }
          end
        end)

    --[[
    var _visitTeam = ["53","利物浦","http://pic.gooooal.com/images/100011/100011218.jpg","100009327","Liverpool FC","1"];
    ]]
    string.gsub(content, 'var%s+_visitTeam%s+=%s%[([^;]+)%]', function(str)
          local t = loadstring('return {' .. str .. '}')()
          if t[1] == tostring(lfc_id) then
            side = 'away'
          else
            opponent_team = {
                id = tonumber(t[1]),
                name = t[2],
                en_name = t[5],
              }
          end
        end)

    --[[
    _personListHost["107179"] = {pid:107179,teamId:216,name:"S.阿奎罗",teamNum:"10",posId:"17",posCode:"F",posName:"前锋",clubId:"216",clubName:"曼城",birthday:"1988-06-02",height:"173",marketValue:"6000万欧元",imgUrl:"http://pic.gooooal.com/images/101115/101115454.jpg",countryImg:"http://pic.gooooal.com/images/100490/100490028.png",countryName:"阿根廷"};
    ]]
    string.gsub(content, '_personList([%w]+)%["(%d+)"%]%s+=%s+({[^}]+})', function(_side, id, str)
            str = string.gsub(str, ':', '=')
            local t = loadstring('return '..str)()
            local player = {
                    pid = t.pid,
                    name = t.name,
                }
            if _side == 'Host' then
                if side == 'home' then
                    lfc_player[id] = player
                else
                    opponent_player[id] = player
                end
            else
                if side == 'away' then
                    lfc_player[id] = player
                else
                    opponent_player[id] = player
                end
            end
        end)
      
    --[[
    _logList.push({id:108106577,teamId:53,periodId:100,logId:202,logTime:1140,personId:100362176,assistId:"",remark:" Alexander-Arnold T.",reason:""});
    ]]
    string.gsub(content, '_logList.push%(({[^}]+})%)', function(str)
            str = string.gsub(str, ':', '=')
            local t = loadstring('return '..str)()
            if t.logId == 1 then -- 进球
                local goal = {
                        type = 'normal',
                        goal_player_id = tonumber(t.personId),
                        assist_player_id = tonumber(t.assistId),
                        time = parse_match_time(t.periodId, t.logTime),
                    }
                
                if t.teamId == lfc_team_id then
                    table.insert(lfc_goals, goal)
                else
                    table.insert(opponent_goals, goal)
                end
            elseif t.logId == 2 then -- 乌龙
                local goal = {
                        type = 'owngoal',
                        goal_player_id = tonumber(t.personId),
                        assist_player_id = 0,
                        time = parse_match_time(t.periodId, t.logTime),
                    }
                
                -- 乌龙球这里要反着算
                if t.teamId == lfc_team_id then
                    table.insert(opponent_goals, goal)
                else
                    table.insert(lfc_goals, goal)
                end
            elseif t.logId == 3 then -- 点球
                local goal = {
                        type = 'penalty',
                        goal_player_id = tonumber(t.personId),
                        assist_player_id = 0,
                        time = parse_match_time(t.periodId, t.logTime),
                    }
                
                if t.teamId == lfc_team_id then
                    table.insert(lfc_goals, goal)
                else
                    table.insert(opponent_goals, goal)
                end
            end
        end)
    
    local lfc_player_arr = {}
    for k,v in pairs(lfc_player) do
        table.insert(lfc_player_arr, v)
    end
    
    local opponent_player_arr = {}
    for k,v in pairs(opponent_player) do
        table.insert(opponent_player_arr, v)
    end
    
    local match_t = {
            side = side,
            date = date,
            opponent_team = opponent_team,
            
            lfc_player = lfc_player_arr,
            opponent_player = opponent_player_arr,
            
            lfc_goals = lfc_goals,
            opponent_goals = opponent_goals,
        }
        
    return match_t
end

function parse_match_time(period_id, time)
    local match_time = {}
    local minute = time / 60
    if period_id == 100 then -- 上半场
        if minute > 45 then
            match_time.time = 45
            match_time.extra_time = minute - 45
        else
            match_time.time = minute
        end
    elseif period_id == 102 then -- 下半场
        if minute > 90 then
            match_time.time = 90
            match_time.extra_time = minute - 90
        else
            match_time.time = minute
        end
    elseif period_id == 103 then -- 加时赛（好像没分上下半场）
        if minute > 120 then
            match_time.time = 120
            match_time.extra_time = minute - 120
        else
            match_time.time = minute
        end
    elseif period_id == 104 then
        match_time.time = minute
        match_time.is_penalty_shootout = true
    else
        error('unhanlde period == ' .. period_id)
    end
    
    return match_time
end