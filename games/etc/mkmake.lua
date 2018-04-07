local function mkmake( title, waves, images, pages )
  local template = [[
ETC=../etc
PAS2LUA=$(ETC)/pas2lua/pas2lua
LUA=~/Develop/lua-5.3.4/src/lua
SOX=sox
RLRLE=$(ETC)/retroluxury/luai $(ETC)/retroluxury/rlrle.lua
BSENC=$(ETC)/bstree/bsenc.lua
TARCLEAN=$(ETC)/tarclean
MKREADME=$(ETC)/mkreadme.lua

ZIP="${TITLE}.zip"
MGW="${TITLE}.mgw"

SRC_WAV=${WAVES}
PCM=$(SRC_WAV:.wav=.pcm)

RLE=${RLES}

PAGES=${PAGES}

ENTRIES=main.bs unit1.bs $(PCM) $(RLE)

%.pcm: %.wav
	$(SOX) $< -B -t s16 -c 1 -r 44100 $(basename $<).pcm

%.bs: %.lua
	$(LUA) $(BSENC) $< $@

all: zip

zip: mgw README.txt ${MANUAL}
	zip -9 $(ZIP) $(MGW) README.txt ${MANUAL}
	touch zip

unit1.lua: src/unit1.pas src/unit1.dfm
	$(PAS2LUA) $< $@ data

mgw: $(ENTRIES)
	tar --format=v7 --xform='s@.*/@@' -cf temp.tar $(ENTRIES)
	$(TARCLEAN) temp.tar
	bzip2 -9 temp.tar
	mv temp.tar.bz2 $(MGW)
	touch mgw

README.txt: bin/Readme.txt
	$(LUA) $(MKREADME) $< > $@

Manual.pdf: $(PAGES:.bmp=.pdf)
	pdftk $+ cat output $@

${IMGRULES}

${PAGERULES}

clean:
	$(RM) $(ZIP) zip $(MGW) mgw unit1.lua main.bs unit1.bs README.txt ${MANUAL} $(PCM) $(RLE) $(PAGES:.bmp=.pdf) $(PAGES:.bmp=.jpg)
]]
  
  local waves = table.concat( waves, ' ' )
  
  local rles = {}
  for _, image in ipairs( images ) do
    rles[ #rles + 1 ] = image:lower():gsub( '(.*%.)(.+)', '%1rle' )
  end
  rles = table.concat( rles, ' ' )
  
  local imgrules = {}
  for _, image in ipairs( images ) do
    local rle = string.format( '%srle', image:gsub( '(.*%.).+', '%1' ) )
    local rlelower = rle:lower()

    if image:match( '.*%.rle' ) then
    elseif image:match( '.-im_background_screen' ) then
      imgrules[ #imgrules + 1 ] = string.format( '%s: %s', rlelower, image )
      imgrules[ #imgrules + 1 ] = string.format( '\t$(RLRLE) --bl %s', image )

      if rle ~= rlelower then
        imgrules[ #imgrules + 1 ] = string.format( '\tmv %s %s', rle, rlelower )
      end

      imgrules[ #imgrules + 1 ] = ''
    elseif image:match( '.-im_background' ) then
      imgrules[ #imgrules + 1 ] = string.format( '%s: %s', rlelower, image )
      imgrules[ #imgrules + 1 ] = string.format( '\t$(RLRLE) --transp 255 0 255 %s', image )

      if rle ~= rlelower then
        imgrules[ #imgrules + 1 ] = string.format( '\tmv %s %s', rle, rlelower )
      end

      imgrules[ #imgrules + 1 ] = ''
    elseif image:match( '.*%.jpg' ) then
      imgrules[ #imgrules + 1 ] = string.format( '%s: %s', rlelower, image )
      imgrules[ #imgrules + 1 ] = string.format( '\t$(RLRLE) %s', image )

      if rle ~= rlelower then
        imgrules[ #imgrules + 1 ] = string.format( '\tmv %s %s', rle, rlelower )
      end

      imgrules[ #imgrules + 1 ] = ''
    elseif image:match( 'im_rivet_%d%d_%d%d%.bmp' ) then
      imgrules[ #imgrules + 1 ] = string.format( '%s: %s', rlelower, image )
      imgrules[ #imgrules + 1 ] = string.format( '\t$(RLRLE) %s', image )

      if rle ~= rlelower then
        imgrules[ #imgrules + 1 ] = string.format( '\tmv %s %s', rle, rlelower )
      end

      imgrules[ #imgrules + 1 ] = ''
    else
      imgrules[ #imgrules + 1 ] = string.format( '%s: %s', rlelower, image )
      imgrules[ #imgrules + 1 ] = string.format( '\t$(RLRLE) --bl %s', image )

      if rle ~= rlelower then
        imgrules[ #imgrules + 1 ] = string.format( '\tmv %s %s', rle, rlelower )
      end

      imgrules[ #imgrules + 1 ] = ''
    end
  end
  imgrules = table.concat( imgrules, '\n' )
  
  local pagerules = {}
  for _, page in ipairs( pages ) do
    pagerules[ #pagerules + 1 ] = string.format( '%sjpg: %s', page:gsub( '(.*%.).+', '%1' ), page )
    pagerules[ #pagerules + 1 ] = string.format( '\tmogrify -format jpg $<' )
    pagerules[ #pagerules + 1 ] = ''
    pagerules[ #pagerules + 1 ] = string.format( '%spdf: %sjpg', page:gsub( '(.*%.).+', '%1' ), page:gsub( '(.*%.).+', '%1' ) )
    pagerules[ #pagerules + 1 ] = string.format( '\tsam2p $< $@' )
    pagerules[ #pagerules + 1 ] = ''
  end
  pagerules = table.concat( pagerules, '\n' )
  
  local manual = '' -- #pages ~= 0 and 'Manual.pdf' or ''
  
  pages = table.concat( pages, ' ' )
  
  local readme = template:gsub( '%${TITLE}', title ):gsub( '%${WAVES}', waves )
  local readme = readme:gsub( '%${RLES}', rles ):gsub( '%${IMGRULES}', imgrules )
  local readme = readme:gsub( '%${PAGES}', pages ):gsub( '%${PAGERULES}', pagerules )
  local readme = readme:gsub( '%${MANUAL}', manual )
  return readme
end

local function gettitlever( readme )
  local title, version = readme:match( 'Product:%s+(.+)%s+Simulator%s*\r?\nVersion:%s+(.+)%s*\r?\nStarted.*' )
  title = title:gsub( '%s+', ' ' )
  version = version:gsub( '%s+', ' ' )
  return title, version
end

local file = io.open( 'bin/Readme.txt' )
local title, version = gettitlever( file:read( '*a' ) )
file:close()

local waves = {}
file = io.popen( 'ls bin/Sound/*.wav' )
for line in file:lines() do
  waves[ #waves + 1 ] = line
end
file:close()

local images = {}
file = io.popen( 'ls data/*' )
for line in file:lines() do
  images[ #images + 1 ] = line
end
file:close()

local pages = {}
file = io.popen( 'ls manual/im_page*.bmp' )
for line in file:lines() do
  pages[ #pages + 1 ] = line
end
file:close()

file = io.open( 'Makefile', 'w' )
file:write( mkmake( title, waves, images, pages ) )
file:close()
