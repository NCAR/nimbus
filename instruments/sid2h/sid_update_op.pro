PRO sid_update_op, op
   ;Updates the 'op' structure if any new tags are added
   IF total(where(tag_names(op) eq 'AUTOLEVEL')) eq -1 THEN op=create_struct(op,'autolevel',0)
   IF total(where(tag_names(op) eq 'SPEEDREJECT')) eq -1 THEN op=create_struct(op,'speedreject',0)
   IF total(where(tag_names(op) eq 'TEXTFILE')) eq -1 THEN op=create_struct(op,'textfile',0)
   IF total(where(tag_names(op) eq 'PTHFILE')) eq -1 THEN op=create_struct(op,'pthfile','none')
   IF total(where(tag_names(op) eq 'FIXEDTAS')) eq -1 THEN op=create_struct(op,'fixedtas',150.0)
   IF total(where(tag_names(op) eq 'RATE')) eq -1 THEN op=create_struct(op,'rate',1)
   IF total(where(tag_names(op) eq 'SIZEGAIN')) eq -1 THEN op=create_struct(op,'sizegain',0.2)
   IF total(where(tag_names(op) eq 'OUTDIR')) eq -1 THEN op=create_struct(op,'outdir',file_dirname(op.fn_orig[0],/mark))
   IF total(where(tag_names(op) eq 'INTTHRESHOLD')) eq -1 THEN op=create_struct(op,'intthreshold',1.0e-6)
   IF total(where(tag_names(op) eq 'MAXSATURATED')) eq -1 THEN op=create_struct(op,'maxsaturated',10)
   IF total(where(tag_names(op) eq 'SPEEDREJECT')) eq -1 THEN op=create_struct(op,'speedreject',0)
   IF total(where(tag_names(op) eq 'PEAK')) eq -1 THEN op=create_struct(op,'peak',0)
   IF total(where(tag_names(op) eq 'CREATENCDF')) eq -1 THEN op=create_struct(op,'createncdf',0)
   IF total(where(tag_names(op) eq 'CREATESAV')) eq -1 THEN op=create_struct(op,'createsav',1)
   IF total(where(tag_names(op) eq 'NCAPPEND')) eq -1 THEN op=create_struct(op,'ncappend',0)
   IF total(where(tag_names(op) eq 'FINALPROCESSING')) eq -1 THEN op=create_struct(op,'finalprocessing',0)
   IF total(where(tag_names(op) eq 'CLOCKOFFSET')) eq -1 THEN op=create_struct(op,'clockoffset',0)
   IF total(where(tag_names(op) eq 'STARTTIME')) eq -1 THEN op=create_struct(op,'starttime',-1)
   IF total(where(tag_names(op) eq 'STOPTIME')) eq -1 THEN op=create_struct(op,'stoptime',-1)
END
  