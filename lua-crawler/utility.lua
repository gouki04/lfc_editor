module('utility', package.seeall)

function format_table(tt, indent, done)
    done = done or {}
    indent = indent or 0
    if type(tt) == "table" then
        local sb = {}
        table.insert(sb, '{\n')
        for key, value in pairs (tt) do
            table.insert(sb, string.rep ("    ", indent)) -- indent it
            if type(key) == 'number' then
                table.insert(sb, '[')
                table.insert(sb, key)
                table.insert(sb, ']')
            else
                table.insert(sb, key)
            end
            
            table.insert(sb, ' = ')
            if type (value) == "table" and not done [value] then
                done [value] = true
                table.insert(sb, format_table(value, indent + 1, done))
            elseif type(value) == 'number' then
                table.insert(sb, value)
            elseif type(value) == 'string' then
                table.insert(sb, '"')
                table.insert(sb, value)
                table.insert(sb, '"')
            else
                error('not support type == ' .. type(value))
            end
            table.insert(sb, ',\n')
        end
        table.insert(sb, string.rep ("    ", indent-1))
        table.insert(sb, '}')
        return table.concat(sb)
    else
        return tt .. "\n"
    end
end

function save_table_to_file(t, file_name)
    local file_content = 'return ' .. format_table(t)
    local file = io.open(file_name, 'w')
    file:write(file_content)
    file:flush()
    file:close()
    
    return true
end

function load_table_from_file(file_name)
    local file = io.open(file_name, 'r')
    if file == nil then
        return nil
    end
    
    local file_content = file:read('*a')
    file:close()
    
    return loadstring(file_content)()
end

function save_json_to_file(t, file_name)
    local json = require 'cjson'
    local file_content = json.encode(t)
    local file = io.open(file_name, 'w')
    file:write(file_content)
    file:flush()
    file:close()
    
    return true
end

function load_json_from_file(file_name)
    local file = io.open(file_name, 'r')
    if file == nil then
        return nil
    end
    
    local file_content = file:read('*a')
    file:close()
    
    local json = require 'cjson'
    return json.decode(file_content)
end