-- Apply the script to the Alliance and Horde Zeppelins
UPDATE creature_template SET ScriptName = 'npc_ac130_zeppelin' WHERE entry IN (34051, 34036);

-- Ensure they are set to Hover/Fly
UPDATE creature_template SET InhabitType = 4, HoverHeight = 80, FlagsExtra = 2 WHERE entry IN (34051, 34036);