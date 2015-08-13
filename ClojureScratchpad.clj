;; Clojure scratchpad
;; Jon Fast
;; 8/9/2015

;; Basic if statements
(defn galaxy-quest [truth] (do (println(if truth
  (do (println "Success!") "By Grapthar's hammer!")
  (do (println "Failure... :(") "By the sons of Warban, you shall be avenged!")))))
(galaxy-quest false)
(galaxy-quest true)

;; Strings
(def blahString "UGHUGHUGHUGHUHGUHGUHG")
(println (str "This is what sound I make on Mondays - " blahString "!"))

;; Maps
(def myMap {:something "blah", :somethingElse "blahblah", :somethingSomethingElse {:a "blahb"}})
(println(get (get myMap :somethingSomethingElse) :a))
(println(get myMap :somethingElse))

;; Vectors
(println (get [3, 2, 1] 1))

;; Lists
(println '(1 2 3 4))
(println (nth '(1 2 3 4) 2))