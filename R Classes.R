e <- new.env()
e$employee <- setRefClass('employee',
                        fields = list(
                          number = "numeric",
                          name = "character",
                          rate = "numeric",
                          hoursWorked = "numeric",
                          department = "character"
                        ),
                        methods = list(
                          copyEmployee = function(anotherEmployee) {
                            if (class(anotherEmployee) == class(.self)) {
                              .self$setNumber(anotherEmployee$number)
                              .self$setName(anotherEmployee$name)
                              .self$setRate(anotherEmployee$rate)
                              .self$setDepartment(anotherEmployee$department)
                              .self$setHoursWorked(anotherEmployee$hoursWorked)
                              .self
                            } else { 
                              stop(paste("Cannot convert", class(anotherEmployee), "to an employee"))
                            }
                          },
                          setNumber = function(number) {
                            number <<- number
                          },
                          setName = function(name) {
                            name <<- name
                          },
                          setRate = function(rate) {
                            rate <<- rate
                          },
                          setDepartment = function(department) {
                            department <<- department
                          },
                          setHoursWorked = function(hoursWorked) {
                            hoursWorked <<- hoursWorked
                          },
                          getSalary = function() {
                            rate * hoursWorked
                          }
                        ))

emp1 <- e$employee(number = 10123,
                   name = "Jon F",
                   rate = 20,
                   hoursWorked = 0,
                   department = "IT Tech Support")
emp2 <- e$employee(number = 10124,
                   name = "Somebody",
                   rate = 10,
                   hoursWorked = 32,
                   department = "Not IT")