local crawler = require 'crawler'
local parser = require 'parser'
local lfs = require 'lfs'

local crawler_base_dir = lfs.currentdir() .. '/data/html/'
lfs.mkdir(crawler_base_dir)

crawler.init(crawler_base_dir)
crawler.crawl_match_by_year(2011)

local parser_base_dir = lfs.currentdir() .. '/data/json/'
lfs.mkdir(parser_base_dir)

parser.init(parser_base_dir)
parser.parser_match_by_year(2011)

print ('finish')