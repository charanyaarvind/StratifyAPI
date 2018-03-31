#ifndef TEST_TEST_HPP
#define TEST_TEST_HPP

#include "../api/SObject.hpp"
#include "../sys/MicroTime.hpp"
#include "../sys/Timer.hpp"

namespace test {

/*! \brief Test Class
 * \details The Test class is designed to
 * be inherited to implement the following
 * tests:
 *
 * - API Tests: Should be designed to ensure the API is documented correctly and works as expected
 * - Performance Tests: Should be designed to run the same set of operations over and over
 * - Stress Tests: Should be designed to stress the system and check for unexpected behavior
 * - Additional Tests: Used as needed by inheriting classes to run additional tests
 *
 *
 * \code
 *
 * #include <sapi/test.hpp>
 *
 * u32 o_execute_flags = Test::EXECUTE_ALL;
 * Test::initialize(cli.name(), cli.version());
 * if( o_flags & DATA_TEST_FLAG ){
 *   Test test("generic test");  //Must be constructed after Test::inialize()
 *   test.execute(o_execute_flags);
 *   //At this point test will be deconstructed -- must happen before Test::finalize()
 * }
 * Test::finalize();
 * \endcode
 *
 *
 */
class Test : public api::SObject {
public:


    /*! \details Initializes the test report.
     *
     * This must be called before any tests are even
     * constructed.
     *
     */
    static void initialize(const char * name, const char * version);

    /*! \details Finalizes the test report.
     *
     * This should be called after the last test
     * has been deconstructed.
     *
     */
    static void finalize();

    /*! \details Constructs a new test object.
     *
     * @param name The name of the test
     *
     * <b>The test must be constructed after Test::initialize().</b>
     *
     * \code
     *
     * //the WRONG way
     * int main(int argc, char * argv[]){
     *   Test my_test("my empty test"); //DON'T CONSTRUCT BEFORE Test::initialize()
     *   Test::initialize();
     *   my_test.execute();
     *   Test::finalize();
     *
     *   return 0; //my_test is deconstructed after Test::finalize() -- WRONG!
     * }
     *
     * //the RIGHT way
     * int main(int argc, char * argv[]){
     *   bool is_run_test = true;
     *   Test::initialize();
     *   if(is_run_test){
     *     Test my_test("my empty test"); //DON'T CONSTRUCT BEFORE Test::initialize()
     *     my_test.execute();
     *     //my_test will deconstruct here BEFORE Test::finalize() --RIGHT
     *   }
     *   Test::finalize();
     *   return 0;
     * }
     *
     *
     *
     * \endcode
     *
     */
    Test(const char * name);

    /*! \details Deconstructs the test object.
     *
     * The object must be deconstructed before Test::finalize()
     *
     *
     */
    ~Test();


    enum {
        EXECUTE_API /*! API Test Execution flag */ = (1<<0),
        EXECUTE_STRESS /*! Stress Test Execution flag */ = (1<<1),
        EXECUTE_PERFORMANCE /*! Performance Test Execution flag */ = (1<<2),
        EXECUTE_ADDITIONAL /*! Additional Test Execution flag */ = (1<<3),
        EXECUTE_ALL /*! API Execution flag */ = (int)-1
    };

    /*! \details Executes the tests specified by \a o_flags.
     *
     * @param o_flags Bitmaks of the tests to execute (e.g., EXECUTE_API)
     *
     */
    virtual void execute(int o_flags = EXECUTE_ALL){
        if( o_flags & EXECUTE_API ){ execute_api_case(); }
        if( o_flags & EXECUTE_STRESS ){ execute_stress_case(); }
        if( o_flags & EXECUTE_PERFORMANCE ){ execute_performance_case(); }
        if( o_flags & EXECUTE_ADDITIONAL ){ execute_additional_cases(); }
    }

    /*! \details Executes the API test case. */
    void execute_api_case();
    /*! \details Executes the performance test case. */
    void execute_performance_case();
    /*! \details Executes the stress test case. */
    void execute_stress_case();
    /*! \details Executes additional test cases.
     *
     * By default there are not additional cases. The
     * inheriting class should reimplement
     * this method it additional test cases are used.
     *
     * \code
     * void MyClassTest::execute_additional_cases(){
     *   open_case("first test case");
     *   set_case_message("message", "test case message");
     *   close_case(true); //case passed
     *
     *   open_case("second test case");
     *   set_case_message("message", "test case message");
     *   close_case(false); //case failed
     * }
     *
     * \endcode
     *
     */
    virtual void execute_additional_cases();

    /*! \details Executes the class api test.
     *
     * This method should be overridden by the inheriting
     * class.
     *
     */
    virtual bool execute_class_api_case();

    /*! \details Executes the class performance test.
     *
     * This method should be overridden by the inheriting
     * class.
     *
     */
    virtual bool execute_class_performance_case();

    /*! \details Executes the class stress test.
     *
     * This method should be overridden by the inheriting
     * class.
     *
     */
    virtual bool execute_class_stress_case();

protected:

    /*! \details Returns a reference to the case timer.
     *
     * The case timer is used to time each case. It
     * is started and stopped automatically.
     *
     * Test methods can access the value of the case timer
     * while the test is running.
     *
     */
    const sys::Timer & case_timer() const { return m_case_timer; }

    /*! \details Opens a new test case.
     *
     * @param case_name The name of the case
     *
     * This method is only used when re-implementing
     * execute_additional_cases().
     *
     */
    void open_case(const char * case_name);

    /*! \details Closes a new test case.
     *
     * @param result true if the case passed and false if it failed
     *
     * This method is only used when re-implementing
     * execute_additional_cases().
     *
     */
    void close_case(bool result);


    /*! \details Sets a message to be stored with the case.
     *
     * @param key The message key identifier (any string)
     * @param message The message to store
     *
     * This method can be used for both successful and
     * unsuccessful cases. For unsuccessful cases,
     * if can save in the test report why the test failed.
     *
     * This method can be used with set_case_value() to
     * save information about test execution.
     *
     */
    void set_case_message(const char * key, const char * message);

    /*! \details Sets a case value (u32).
     *
     * @param key The key to identify the value
     * @param value The value to save
     *
     * \sa set_case_message()
     *
     */
    void set_case_value(const char * key, u32 value);

    /*! \details Sets a case value (float).
     *
     * @param key The key to identify the value
     * @param value The value to save
     *
     * \sa set_case_message()
     *
     */
    void set_case_value(const char * key, float value);

    /*! \details Sets a case value (s32).
     *
     * @param key The key to identify the value
     * @param value The value to save
     *
     * \sa set_case_message()
     *
     */
    void set_case_value(const char * key, s32 value);

    /*! \details Sets a case value (int).
     *
     * @param key The key to identify the value
     * @param value The value to save
     *
     * \sa set_case_message()
     *
     */
    void set_case_value(const char * key, int value);

private:

    //sys::MicroTime m_execution_time;
    bool m_test_result;
    sys::Timer m_case_timer;
    sys::Timer m_test_timer;
    bool m_is_first_case;
    static bool m_is_initialized;
    static bool m_is_first_test;

};

}

#endif // TEST_TEST_HPP
