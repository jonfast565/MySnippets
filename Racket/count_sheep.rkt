#lang racket

; Tell me how many sheep have been counted
(define (sheep-number n) 
  (displayln 
   (string-append "Sheep: " 
                  (number->string n))))

; Tell me how to count the sheep
(define (count-sheep n time)
  (sleep time)
  (sheep-number n)
  (cond [(= n 0) (displayln "Sleep!")]
        [(< n 0) (displayln "Negative sheep... blaaaaah.")]
        [(> n 0) (count-sheep (sub1 n) time)]))

; Run our sheep counter
(thread (lambda () (count-sheep 100 1)))
