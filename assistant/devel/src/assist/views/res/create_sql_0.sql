create table adx_params 
(
    name varchar(256), 
    value varchar(256)
);
    
create table adx_modules 
(
    id integer primary key,
    name varchar(256),
    version char(10)
);

create table adx_items 
(
    id integer primary key, 
    created timestamp,
    modified timestamp,
    name text,
    content text, 
    module integer,
    deleted integer default 0,
    folder integer default 0,
    parent integer default 0
 );
     
create index adx_idx_folders_parent on adx_items (parent);