
;;;  als aan echte stok
;;;      if ((note_column_drul[d] == get_bound (d))
;;;	  && note_column_drul[d]->first_head ()
;;;	  && (note_column_drul[d]->stem_l ()))
;;;
;;;  *Need: Score_elment::pointer_alist_
;;;         Score_elment::property_alist_
;;;         Spanner::Drul_array<Item*> spanned_drul_;
;;;         spanner:: (cons get_bound (LEFT) get_bound (RIGHT))

(define (attached-to-stem slur dir)
  (let* ((note-columns (get-pointer slur 'note-columns))
	 (col (if (= dir 1) (car note-columns) (car (reverse note-columns))))
	 (stem (get-pointer col 'stem)))
    (and
     (eq? col (get-bound slur dir))
     stem
     (get-pointer stem 'heads))))

(define slur-extremity-rules
  '(
    ;;if (stem_l->beam_l () && (stem_l->beam_count (-d) >= 1))
    ((lambda (slur dir)
       ;; urg, code dup
       ;; if attached-to-stem
       (let* ((note-columns (get-pointer slur 'note-columns))
	 (col (if (= dir 1) (car note-columns) (car (reverse note-columns))))
	 (stem (get-pointer col 'stem)))
	 (and
	  (and
	   (eq? col (get-bound slur dir))
	   stem
	   (get-pointer stem 'heads))
	  ;; and got beam
	  (and (get-pointer stem 'beam)
	       ;; and beam on same side as slur
	       (let ((beaming (get-property stem 'beaming)))
		 (if (pair? beaming)
		     (>= 1
			 (if (= dir -1) (car beaming) (cdr beaming)))
		     #f)))))) . stem)
    ((lambda (slur dir) (not (attached-to-stem slur dir))) . loose-end)
    ;; default case, attach to head
    ((lambda (x y) #t) . head)
    
  ;; silly rule, just to check
  ((lambda (slur dir)
       (and (attached-to-stem slur dir) 
	    (= (get-property slur 'direction) dir))) . stem)
  ))


(define slur-extremity-offset-alist
  '(
    ((head 1 1) . (-0.25 . 0.2))
    ((head 1 -1) . (-0.25 . -0.75))
    ((head -1 1) . (-0.25 . 0.75))
    ((head -1 -1) . (-0.75 . 1.2))
    ((stem 1 1) . (0 . 0.2))
    ((stem -1 -1) . (0 . 0.2))
    ))
